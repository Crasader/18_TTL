USE [QPTreasureDB]
GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_GetPlayerRecordList]    Script Date: 2017/10/31 19:00:13 ******/

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetPlayerRecordToTalList]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROC [dbo].GSP_GP_GetPlayerRecordToTalList
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetPlayerRecordSmallList]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROC [dbo].GSP_GP_GetPlayerRecordSmallList
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<ttf>
-- Create date: <2017-10-5 13:19:59>
-- Description:	<查询玩家战绩>
-- =============================================
CREATE PROC GSP_GP_GetPlayerRecordToTalList
	(
		@UserID bigint
	)
WITH ENCRYPTION AS

BEGIN
	SET NOCOUNT ON

	BEGIN TRAN

	BEGIN TRY

	--#临时表 生命周期 连接存在时一直存在
	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableIDs'))
	DROP TABLE #TableIDs;

	--SELECT TOP 20 TableID, UserID, SUM(DrawID) AS DrawIDs
	--	FROM dbo.RecordDrawScore
	--	WHERE UserID = @UserID
	--	GROUP BY TableID, UserID
	--	ORDER BY DrawIDs DESC

	SELECT TOP 20 
			KindID,
			[dbo].[RecordDrawScore].TableID,
			UserID,
			SUM([dbo].[RecordDrawScore].DrawID) AS DrawIDs
		INTO [#TableIDs]
		FROM [dbo].[RecordDrawScore]
			RIGHT JOIN [dbo].[RecordDrawInfo] ON [dbo].[RecordDrawScore].TableID = [dbo].[RecordDrawInfo].TableID
		WHERE [dbo].[RecordDrawScore].UserID = @UserID
		GROUP BY KindID,[dbo].[RecordDrawScore].TableID, [dbo].[RecordDrawScore].UserID
		ORDER BY DrawIDs DESC

	SELECT	[#TableIDs].KindID,
			[dbo].[RecordDrawScore].TableID,
			[dbo].[RecordDrawScore].UserID,
			[QPAccountsDB].[dbo].[AccountsInfo].NickName,
			Score=SUM([dbo].[RecordDrawScore].Score),
			DrawIDs=SUM([dbo].[RecordDrawScore].DrawID)
		FROM [dbo].[RecordDrawScore]
			RIGHT JOIN #TableIDs ON [dbo].[RecordDrawScore].TableID = [#TableIDs].TableID
			LEFT JOIN [QPAccountsDB].[dbo].[AccountsInfo] ON [dbo].[RecordDrawScore].UserID = [QPAccountsDB].[dbo].[AccountsInfo].UserID
		GROUP BY [#TableIDs].KindID,
				[dbo].[RecordDrawScore].TableID,
				[dbo].[RecordDrawScore].UserID,
				[QPAccountsDB].[dbo].[AccountsInfo].NickName
		ORDER BY DrawIDs DESC

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableIDs'))
	DROP TABLE #TableIDs;

	END TRY

	BEGIN CATCH

	END CATCH

	COMMIT TRAN

END

GO

 --=============================================
 --Author:		<ttf>
 --Create date: <2017-10-5 13:19:59>
 --Description:	<查询玩家战绩>
 --=============================================
CREATE PROC GSP_GP_GetPlayerRecordSmallList
	(
		@UserID bigint,
		@TableID bigint
	)
WITH ENCRYPTION AS

BEGIN
	SET NOCOUNT ON

	SELECT TableID, DrawID, UserID, Score
		FROM [dbo].[RecordDrawScore]
		WHERE [dbo].[RecordDrawScore].UserID=@UserID and TableID=@TableID

END
GO
