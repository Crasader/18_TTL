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
		@UserID BIGINT,
		@DrawID BIGINT,
		@UpOrDown INT
	)
WITH ENCRYPTION AS

BEGIN
	SET NOCOUNT ON

	DECLARE @Ret INT;

	BEGIN TRAN

	BEGIN TRY

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#DrawIDs'))
	DROP TABLE #DrawIDs

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableID'))
	DROP TABLE #TableID

	DECLARE @strFindTableBegin VARCHAR(max);
	DECLARE @strCondition VARCHAR(max);
	DECLARE @strFindTableEnd VARCHAR(max);

	SET @Ret = 1

	SET @strFindTableBegin=
	'SELECT TOP 1
			[dbo].[RecordDrawScore].TableID,
			[dbo].[RecordDrawScore].UserID,
			[dbo].[RecordDrawScore].StartTime,
			Min([dbo].[RecordDrawScore].DrawID) AS MinDrawID,
			Max([dbo].[RecordDrawScore].DrawID) AS MaxDrawID,
			SUM([dbo].[RecordDrawScore].DrawID) AS DrawIDs,
			COUNT([dbo].[RecordDrawScore].DrawID) AS DrawCount
		FROM [dbo].[RecordDrawScore]
		WHERE [dbo].[RecordDrawScore].UserID = '+ STR(@UserID)

	SET @strCondition = ' '
	IF @UpOrDown = 1
		SET	@strCondition =' AND DrawID < ' + STR(@DrawID)
	IF @UpOrDown = 2
		SET	@strCondition =' AND DrawID > ' + STR(@DrawID)

	SET @strFindTableEnd=
		' GROUP BY [dbo].[RecordDrawScore].TableID,
				[dbo].[RecordDrawScore].UserID,
				[dbo].[RecordDrawScore].StartTime
		ORDER BY StartTime DESC'

	CREATE TABLE #TableID(
		TableID INT,
		UserID INT,
		StartTime BIGINT,
		MinDrawID BIGINT,
		MaxDrawID BIGINT,
		DrawIDs BIGINT,
		DrawCount INT,
	)

	INSERT INTO #TableID EXEC (@strFindTableBegin + @strCondition + @strFindTableEnd)
	--SELECT * FROM [#TableID]

	SET @Ret = 2

	--2.选出所有的局数号,因为TableID不是唯一的
	SELECT  [dbo].[RecordDrawScore].DrawID,
			[dbo].[RecordDrawScore].TableID,
			[dbo].[RecordDrawScore].UserID,
			[#TableID].MinDrawID,
			[#TableID].MaxDrawID,
			[#TableID].DrawCount
		INTO #DrawIDs
		FROM [dbo].[RecordDrawScore]
			JOIN [#TableID] ON [#TableID].TableID = [dbo].[RecordDrawScore].TableID
				AND [dbo].[RecordDrawScore].DrawID >= [#TableID].MinDrawID
				AND [dbo].[RecordDrawScore].DrawID <= [#TableID].MaxDrawID
		WHERE [dbo].[RecordDrawScore].UserID=[#TableID].UserID
	--SELECT * FROM [#DrawIDs]

	SET @Ret = 3

	--3.用选出的局数去找所有的玩家的牌局信息
	SELECT 
		[dbo].[RecordDrawScore].TableID,
		[dbo].[RecordDrawScore].UserID,
		SUM([dbo].[RecordDrawScore].Score) AS Score,
		[dbo].[RecordDrawInfo].KindID,
		[dbo].[RecordDrawInfo].ServerID,
		[dbo].[RecordDrawInfo].BaseScore,
		[dbo].[RecordDrawInfo].RulesBytes,
		[dbo].[RecordDrawInfo].GameType,
		[dbo].[RecordDrawInfo].StartTime,
		[#DrawIDs].MinDrawID,
		[#DrawIDs].MaxDrawID,
		[#DrawIDs].DrawCount
		INTO [#DrawInfo]
		FROM [#DrawIDs]
			LEFT JOIN [dbo].[RecordDrawScore] ON [#DrawIDs].DrawID = [dbo].[RecordDrawScore].DrawID
			LEFT JOIN [dbo].[RecordDrawInfo] ON [#DrawIDs].DrawID = [dbo].[RecordDrawInfo].DrawID
		GROUP BY [dbo].[RecordDrawScore].TableID,
				[dbo].[RecordDrawScore].UserID,
				[dbo].[RecordDrawInfo].KindID,
				[dbo].[RecordDrawInfo].ServerID,
				[dbo].[RecordDrawInfo].BaseScore,
				[dbo].[RecordDrawInfo].RulesBytes,
				[dbo].[RecordDrawInfo].GameType,
				[dbo].[RecordDrawInfo].StartTime,
				[#DrawIDs].MinDrawID,
				[#DrawIDs].MaxDrawID,
				[#DrawIDs].DrawCount

	--4.用选出的牌局信息再去匹配玩家的头像和姓名
	SELECT 
		[#DrawInfo].KindID,
		[#DrawInfo].ServerID,
		[#DrawInfo].TableID,
		[#DrawInfo].StartTime,
		[#DrawInfo].UserID,
		[QPAccountsDB].[dbo].[AccountsInfo].NickName,
		[QPAccountsDB].[dbo].[IndividualDatum].HeadHttp,
		[#DrawInfo].Score,
		[#DrawInfo].BaseScore,
		[#DrawInfo].RulesBytes,
		[#DrawInfo].GameType,
		[#DrawInfo].MinDrawID,
		[#DrawInfo].MaxDrawID,
		[#DrawInfo].DrawCount
		FROM [#DrawInfo]
			LEFT JOIN [QPAccountsDB].[dbo].[AccountsInfo] ON [#DrawInfo].UserID = [QPAccountsDB].[dbo].[AccountsInfo].UserID
			LEFT JOIN [QPAccountsDB].[dbo].[IndividualDatum] ON [#DrawInfo].UserID = [QPAccountsDB].[dbo].[IndividualDatum].UserID
		GROUP BY [#DrawInfo].TableID,
				[#DrawInfo].UserID,
				[QPAccountsDB].[dbo].[AccountsInfo].NickName,
				[QPAccountsDB].[dbo].[IndividualDatum].HeadHttp,
				[#DrawInfo].Score,
				[#DrawInfo].KindID,
				[#DrawInfo].ServerID,
				[#DrawInfo].BaseScore,
				[#DrawInfo].RulesBytes,
				[#DrawInfo].GameType,
				[#DrawInfo].StartTime,
				[#DrawInfo].MinDrawID,
				[#DrawInfo].MaxDrawID,
				[#DrawInfo].DrawCount

	SET @Ret = 4

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#DrawIDs'))
	DROP TABLE #DrawIDs

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableID'))
	DROP TABLE #TableID

	COMMIT TRAN

	SET @Ret = 0

	END TRY

	BEGIN CATCH

		ROLLBACK TRAN

	END CATCH

	RETURN @Ret
END

GO

 --=============================================
 --Author:		<ttf>
 --Create date: <2017-10-5 13:19:59>
 --Description:	<查询玩家战绩>
 --=============================================
CREATE PROC GSP_GP_GetPlayerRecordSmallList
	(
		@UserID BIGINT,
		@DrawID BIGINT,
		@UpOrDown INT
	)
WITH ENCRYPTION AS

BEGIN
	SET NOCOUNT ON

	DECLARE @Ret INT;

	BEGIN TRAN

	BEGIN TRY

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#DrawIDs'))
	DROP TABLE #DrawIDs

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableID'))
	DROP TABLE #TableID

	DECLARE @strFindTableBegin VARCHAR(max);
	DECLARE @strCondition VARCHAR(max);
	DECLARE @strFindTableEnd VARCHAR(max);

	SET @Ret = 1

	SET @strFindTableBegin=
	'SELECT TOP 1
			[dbo].[RecordDrawScore].TableID,
			[dbo].[RecordDrawScore].UserID,
			[dbo].[RecordDrawScore].StartTime,
			Min([dbo].[RecordDrawScore].DrawID) AS MinDrawID,
			Max([dbo].[RecordDrawScore].DrawID) AS MaxDrawID,
			SUM([dbo].[RecordDrawScore].DrawID) AS DrawIDs
		FROM [dbo].[RecordDrawScore]
		WHERE [dbo].[RecordDrawScore].UserID = '+ STR(@UserID)

	SET @strCondition = ' '
	IF @UpOrDown = 1
		SET	@strCondition =' AND DrawID < ' + STR(@DrawID)
	IF @UpOrDown = 2
		SET	@strCondition =' AND DrawID > ' + STR(@DrawID)

	SET @strFindTableEnd=
		' GROUP BY [dbo].[RecordDrawScore].TableID,
				[dbo].[RecordDrawScore].UserID,
				[dbo].[RecordDrawScore].StartTime
		ORDER BY StartTime DESC'

	CREATE TABLE #TableID(
		TableID INT,
		UserID INT,
		StartTime BIGINT,
		MinDrawID BIGINT,
		MaxDrawID BIGINT,
		DrawIDs BIGINT
	)

	INSERT INTO #TableID EXEC (@strFindTableBegin + @strCondition + @strFindTableEnd)
	--SELECT * FROM [#TableID]

	SET @Ret = 2

	--2.选出所有的局数号,因为TableID不是唯一的
	SELECT  [dbo].[RecordDrawScore].DrawID,
			[dbo].[RecordDrawScore].TableID,
			[dbo].[RecordDrawScore].UserID,
			[#TableID].MinDrawID,
			[#TableID].MaxDrawID
		INTO #DrawIDs
		FROM [dbo].[RecordDrawScore]
			JOIN [#TableID] ON [#TableID].TableID = [dbo].[RecordDrawScore].TableID
				AND [dbo].[RecordDrawScore].DrawID >= [#TableID].MinDrawID
				AND [dbo].[RecordDrawScore].DrawID <= [#TableID].MaxDrawID
		WHERE [dbo].[RecordDrawScore].UserID=[#TableID].UserID

	--SELECT * FROM [#DrawIDs]
	SET @Ret = 3

	--3.选出全部的分数
	SELECT [dbo].[RecordDrawScore].DrawID,
		[dbo].[RecordDrawScore].TableID,
		[dbo].[RecordDrawScore].UserID,
		[dbo].[RecordDrawScore].Score,
		[dbo].[RecordDrawInfo].StartTime
		FROM [#DrawIDs]
			LEFT JOIN [dbo].[RecordDrawScore] ON [#DrawIDs].DrawID = [dbo].[RecordDrawScore].DrawID
			LEFT JOIN [dbo].[RecordDrawInfo] ON [#DrawIDs].DrawID = [dbo].[RecordDrawInfo].DrawID
			LEFT JOIN [QPAccountsDB].[dbo].[AccountsInfo] ON [#DrawIDs].UserID = [QPAccountsDB].[dbo].[AccountsInfo].UserID
			LEFT JOIN [QPAccountsDB].[dbo].[IndividualDatum] ON [#DrawIDs].UserID = [QPAccountsDB].[dbo].[IndividualDatum].UserID

	SET @Ret = 4

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#DrawIDs'))
	DROP TABLE #DrawIDs

	IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#TableID'))
	DROP TABLE #TableID

	COMMIT TRAN

	SET @Ret = 0

	END TRY

	BEGIN CATCH

		ROLLBACK TRAN

	END CATCH

	RETURN @Ret
END
GO
