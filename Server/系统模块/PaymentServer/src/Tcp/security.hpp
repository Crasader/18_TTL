#ifndef _SECURITY_HPP_H
#define _SECURITY_HPP_H

#include "stdafx.h"

#include <objbase.h>

#include "ConstTypes.h"
#include "packet.h"

namespace tcp_con {

class Security
{
public:

    Security() 
    {};
    virtual ~Security() 
    {};

public:

    //随机映射
    WORD SeedRandMap(WORD wSeed) {
        DWORD dwHold = wSeed;
        return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
    }

    //映射发送数据
    BYTE MapSendByte(BYTE const cbData) {
        BYTE cbMap = g_SendByteMap[(BYTE)(cbData + m_cbSendRound)];
        m_cbSendRound += 3;
        return cbMap;
    }

    //映射接收数据
    BYTE MapRecvByte(BYTE const cbData) {
        BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
        m_cbRecvRound += 3;
        return cbMap;
    }

    //加密数据
    WORD EncryptBuffer(BYTE* pcbDataBuffer, WORD wDataSize, WORD wBufferSize)
    {
        WORD i = 0;
        //效验参数
        assert(wDataSize >= sizeof(TCP_Head));
        assert(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
        assert(wBufferSize >= (wDataSize + 2 * sizeof(DWORD)));

        //调整长度
        WORD wEncryptSize = wDataSize - sizeof(TCP_Info), wSnapCount = 0;
        if ((wEncryptSize % sizeof(DWORD)) != 0)
        {
            wSnapCount = sizeof(DWORD) - wEncryptSize % sizeof(DWORD);
            memset(pcbDataBuffer + sizeof(TCP_Info) + wEncryptSize, 0, wSnapCount);
        }

        //效验码与字节映射
        BYTE cbCheckCode = 0;
        for (i = sizeof(TCP_Info); i < wDataSize; i++)
        {
            cbCheckCode += pcbDataBuffer[i];
            pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
        }

        //填写信息头
        TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
        pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
        pHead->TCPInfo.wPacketSize = wDataSize;
        pHead->TCPInfo.cbDataKind = DK_ENCRYPT;

        //加密数据
        DWORD dwXorKey = m_dwSendXorKey;
        WORD * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
        DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
        WORD wEncrypCount = (wEncryptSize + wSnapCount) / sizeof(DWORD);
        for (i = 0; i < wEncrypCount; i++)
        {
            *pdwXor++ ^= dwXorKey;
            dwXorKey = SeedRandMap(*pwSeed++);
            dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
            dwXorKey ^= g_dwPacketKey;
        }

        //设置变量
        m_dwSendPacketCount++;
        m_dwSendXorKey = dwXorKey;

        return wDataSize;
    }

    //解密数据
    WORD CrevasseBuffer(BYTE* pcbDataBuffer, WORD wDataSize)
    {
        WORD i = 0;
        //效验参数
        assert(wDataSize >= sizeof(TCP_Head));
        WORD pkgSize = ((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize;
        assert(pkgSize == wDataSize); if (pkgSize != wDataSize) throw (std::logic_error)("Package original length error.");

        //调整长度
        WORD wSnapCount = 0;
        if ((wDataSize % sizeof(DWORD)) != 0)
        {
            wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
            memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
        }

        //提取密钥
        if (m_dwRecvPacketCount == 0)
        {
            assert(wDataSize >= (sizeof(TCP_Head) + sizeof(DWORD)));
            if (wDataSize < (sizeof(TCP_Head) + sizeof(DWORD))) throw (std::logic_error)("Package length error when CrevasseBuffer.");
            m_dwRecvXorKey = *(DWORD *)(pcbDataBuffer + sizeof(TCP_Head));
            m_dwSendXorKey = m_dwRecvXorKey;
            MoveMemory(pcbDataBuffer + sizeof(TCP_Head), pcbDataBuffer + sizeof(TCP_Head) + sizeof(DWORD),
                wDataSize - sizeof(TCP_Head) - sizeof(DWORD));
            wDataSize -= sizeof(DWORD);
            ((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize -= sizeof(DWORD);
        }

        //解密数据
        DWORD dwXorKey = m_dwRecvXorKey;
        DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(TCP_Info));
        WORD  * pwSeed = (WORD *)(pcbDataBuffer + sizeof(TCP_Info));
        WORD wEncrypCount = (wDataSize + wSnapCount - sizeof(TCP_Info)) / 4;
        for (i = 0; i < wEncrypCount; i++)
        {
            if ((i == (wEncrypCount - 1)) && (wSnapCount > 0))
            {
                BYTE * pcbKey = ((BYTE *)& m_dwRecvXorKey) + sizeof(DWORD) - wSnapCount;
                CopyMemory(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
            }
            dwXorKey = SeedRandMap(*pwSeed++);
            dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
            dwXorKey ^= g_dwPacketKey;
            *pdwXor++ ^= m_dwRecvXorKey;
            m_dwRecvXorKey = dwXorKey;
        }

        //效验码与字节映射
        TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
        BYTE cbCheckCode = pHead->TCPInfo.cbCheckCode;;
        for (i = sizeof(TCP_Info); i < wDataSize; i++)
        {
            pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
            cbCheckCode += pcbDataBuffer[i];
        }
        if (cbCheckCode != 0) 
			throw (std::logic_error)("Package verify code error.");

        //照原MFC的逻辑加上了这一句，只能说原写法太奇葩
        m_dwRecvPacketCount++;

        return wDataSize;
    }

private:

    BYTE							        m_cbSendRound = 0;						//字节映射
    BYTE							        m_cbRecvRound = 0;						//字节映射
    DWORD							m_dwSendXorKey = 0x12345678;	//发送密钥
    DWORD							m_dwRecvXorKey = 0x12345678;	//接收密钥

    DWORD							m_dwSendTickCount = 0;					//发送时间
    DWORD							m_dwRecvTickCount = 0;					//接受时间
    DWORD							m_dwSendPacketCount = 0;				//发送计数
    DWORD							m_dwRecvPacketCount = 0;				//接受计数
};
}

#endif