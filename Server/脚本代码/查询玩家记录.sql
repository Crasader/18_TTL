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

BEGIN
	SET NOCOUNT ON

	DECLARE @strSql varchar(1024)

	SET @strSql = 'SELECT TOP 20 
					UserID,
					RecordDrawInfo.DrawID,
					KindID,
					TableID, 
					Score,
					Time=RecordDrawInfo.InsertTime
					From [dbo].[RecordDrawInfo] JOIN [dbo].[RecordDrawScore] 
					ON RecordDrawInfo.DrawID=RecordDrawScore.DrawID
					WHERE UserID='+
					STR(@UserID)+
					'ORDER BY DrawID DESC';

	exec(@strSql);

END
GO
