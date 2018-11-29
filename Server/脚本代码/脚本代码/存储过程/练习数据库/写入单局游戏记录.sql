USE QPRecordDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_WriteRoundWinLose]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_WriteRoundWinLose]
GO

-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<taotingfu>
-- Create date: <2017.9.22>
-- Description:	<充值完成后，私人场查询玩家金钱>
-- =============================================
CREATE PROCEDURE GSP_GP_WriteRoundWinLose
	-- Add the parameters for the stored procedure here
	
	--@dwroundId BIGINT,
	@dwRoomId INT,
	@dwUserId INT,
	--@EndTime datetime,
	@dwGameType INT,
	@dwBeginGold INT,
	@dwWinloseGold INT
	
WITH ENCRYPTION AS

BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	DECLARE @dwroundId BIGINT
	DECLARE @EndTime DATETIME
	
/* 	[roundId] [int] IDENTITY(1,1) NOT NULL,
	[roomId] [int] NOT NULL,
	[userId] [int] NOT NULL,
	[endTime] [datetime] NOT NULL,
	[gameType] [int] NOT NULL,
	[beginGold] [int] NOT NULL,
	[winloseGold] [int] NOT NULL, */
	
	SELECT @dwroundId=COUNT(roundId) from dbo.RecordWinLosePerRound
	SET @EndTime=GETDATE()
	
	SET IDENTITY_INSERT dbo.RecordWinLosePerRound ON
	-- 插入记录
	INSERT dbo.RecordWinLosePerRound(roundId,roomId,userId,endTime,gameType,beginGold,winloseGold)
	
	VALUES (@dwroundId,
	@dwRoomId,
	@dwUserId,
	@EndTime,
	@dwGameType,
	@dwBeginGold,
	@dwWinloseGold)
	
	SET IDENTITY_INSERT dbo.RecordWinLosePerRound OFF
END

RETURN 0

GO
