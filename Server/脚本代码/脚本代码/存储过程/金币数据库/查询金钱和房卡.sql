-- 
USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_RefreshTable]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROC [dbo].[GSP_GP_RefreshTable]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

-- =============================================
-- Author:		<taotingfu>
-- Create date: <2017.9.14>
-- Description:	<��ֵ��ɺ�˽�˳���ѯ��ҽ�Ǯ>
-- =============================================

CREATE PROC dbo.GSP_GP_RefreshTable
	@dwUserID INT,							--���ID
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

SET NOCOUNT ON

BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	
	SELECT	Score, InsureScore  FROM QPTreasureDB.dbo.GameScoreInfo(NOLOCK) 
	
	WHERE UserID=@dwUserID

END

RETURN 0

GO
