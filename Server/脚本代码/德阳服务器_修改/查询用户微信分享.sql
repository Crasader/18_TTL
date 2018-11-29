USE [QPTreasureDB]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RequestWeiXinShare]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RequestWeiXinShare]
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[GSP_GR_RequestWeiXinShare]
	@intUserID INT,
	@intShareType INT,
	@strErrorDescribe NVARCHAR(127) OUTPUT
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

DECLARE @WxShareCount INT
DECLARE @WxShareDate DATETIME
DECLARE @WxShareDay CHAR(10)
DECLARE @WxShareCurDay CHAR(10)
DECLARE @Score BIGINT
-- 执行逻辑
BEGIN
	SELECT @WxShareCount=WeiXinShareCount, @WxShareDate=WeiXinShareDate, @Score=Score FROM [dbo].[GameScoreInfo]
		WHERE UserID=@intUserID

	IF @WxShareDate=NULL
	BEGIN
		UPDATE [dbo].[GameScoreInfo] SET Score=@Score+1,WeiXinShareCount=1,WeiXinShareDate=GETDATE() WHERE UserID=@intUserID
	END
	ELSE
	BEGIN
		SET @WxShareCurDay=CONVERT(CHAR(10),GETDATE(),20) 
		SET @WxShareDay=CONVERT(CHAR(10),@WxShareDate,20) 
		IF @WxShareCurDay=@WxShareDay
		BEGIN
			IF @WxShareCount<3
			BEGIN
				UPDATE [dbo].[GameScoreInfo] SET Score=@Score+1,WeiXinShareCount=@WxShareCount+1,WeiXinShareDate=GETDATE() WHERE UserID=@intUserID
			END
		END
		ELSE
		BEGIN
			UPDATE [dbo].[GameScoreInfo] SET Score=@Score+1,WeiXinShareCount=1,WeiXinShareDate=GETDATE() WHERE UserID=@intUserID
		END
	END
END

RETURN 0

GO