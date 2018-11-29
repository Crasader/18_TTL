
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetShopInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_GetShopInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_BuyOder]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_BuyOder]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_GetShopInfo
	
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

SELECT * FROM ShopInfoList
		
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_BuyOder
	@dwUserID INT,							-- �û��ʺ�
	@dwItemID INT,							-- ��Ʒid
	@total_fee INT,							-- ���
	@appid NVARCHAR(32),					-- ΢��id
	@mch_id NVARCHAR(32),					-- �̻���
	@prepay_id NVARCHAR(128),					-- ׼��������
	@strOSType NVARCHAR(32),					-- ϵͳ��
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
	
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	DECLARE @ItemType 	INT
	DECLARE @Price 		INT
	DECLARE @GoodsNum 	INT
	SELECT @ItemType=ItemType, @GoodsNum=GoodsNum, @Price=Price
	FROM ShopInfoList WHERE ItemID=@dwItemID
	
	if @ItemType is NULL
	BEGIN
		set @strErrorDescribe = N'δ�ҵ���Ʒ'
		RETURN 1
	END
	
	DECLARE @LastRecordID INT
	SELECT @LastRecordID=RecordID
	FROM QPRecordDBLink.QPRecordDB.dbo.RecordBuyOder WHERE OderID=@prepay_id
	if @LastRecordID is not NULL
	BEGIN
		set @strErrorDescribe = N'δ֪����'
		RETURN 2
	END
	
	if @ItemType = 0
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score+@GoodsNum WHERE UserID=@dwUserID
	END
	
	if @ItemType = 1
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@GoodsNum WHERE UserID=@dwUserID
	END
	
	
	-- ��ѯ�û�
	DECLARE @NickName NCHAR(32)
	DECLARE	@SpreaderID INT
	SELECT @NickName=NickName, @SpreaderID=SpreaderID
	FROM AccountsInfo WHERE UserID=@dwUserID
	
	INSERT INTO QPRecordDBLink.QPRecordDB.dbo.RecordBuyOder(UserID,OderType,UserName,OderID,ShopID,CostMoney,GetNums,GetType,SpreaderID)
	VALUES(@dwUserID,N'',@NickName,@prepay_id,@dwItemID,@Price,@GoodsNum,@ItemType,@SpreaderID)
	
	UPDATE QPAccountsDBLink.QPAccountsDB.dbo.AccountsBack SET money=money+@Price WHERE userId=@SpreaderID
	
	set @strErrorDescribe = N'��ֵ�ɹ�'
END

RETURN 0

GO
