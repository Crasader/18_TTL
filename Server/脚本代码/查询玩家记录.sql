USE [QPTreasureDB]
GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_GetPlayerRecordList]    Script Date: 2017/10/31 19:00:13 ******/

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetPlayerRecordList]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROC [dbo].[GSP_GP_GetPlayerRecordList]
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<ttd>
-- Create date: <2017-10-5 13:19:59>
-- Description:	<查询玩家战绩>
-- =============================================
CREATE PROC GSP_GP_GetPlayerRecordList
	(
		@UserID bigint
	)

WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

BEGIN

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..##a'))
	DROP TABLE ##a;
	
	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#temp'))
	DROP TABLE #temp;
	
	-- 获取所有与该玩家玩过游戏的玩家的所有数据 临时表##a
	declare @sqlbegin varchar(max);
	declare @sqlend varchar(max);
	set @sqlbegin = 'SELECT record1.DrawID AS DrawID, record1.UserID AS UserID into ##a FROM [QPTreasureDB].[dbo].[RecordDrawScore] as record1,[QPTreasureDB].[dbo].[RecordDrawScore] as record2  where record1.DrawID = record2.DrawID and record2.UserID = ';
	set @sqlend =' group by record1.DrawID, record1.UserID'
	exec(@sqlbegin + @UserID + @sqlend);

	-- 按桌将玩家记录汇总 临时表#temp
	SELECT [KindID] AS KindID,
		   [ServerID] AS ServerID,
		   [TableID] AS TableID,
		   ##a.UserID AS UserID,
		   ##a.DrawID AS DrawID,
		   SUM(sc.score) AS Score,
		   MAX(rdi.InsertTime) AS Time 
		   into #temp
		   FROM [QPTreasureDB].[dbo].[RecordDrawInfo] AS rdi
		   INNER JOIN ##a 
		   ON rdi.DrawID = ##a.DrawID
		   LEFT JOIN [QPTreasureDB].[dbo].[RecordDrawScore] AS sc ON
				sc.UserID = ##a.UserID AND sc.DrawID = ##a.DrawID AND record1.Grade = 0
		   GROUP BY TableID,KindID,##a.UserID,##a.DrawID,ServerID;

	-- 获取玩家个人信息
	SELECT KindID,
		   ServerID,
		   TableID,
		   #temp.UserID,
		   #temp.DrawID,
		   NickName,
		   Score,
		   Time 
		   FROM #temp
		   INNER JOIN [QPAccountsDB].[dbo].[AccountsInfo]
		   ON #temp.UserID = [QPAccountsDB].[dbo].[AccountsInfo].UserID ORDER BY Time DESC;

	DROP TABLE ##a;
	DROP TABLE #temp;
END