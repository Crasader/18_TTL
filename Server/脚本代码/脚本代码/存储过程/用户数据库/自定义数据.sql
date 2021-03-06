

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_PublicNotic]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_PublicNotic]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LOAD_GAME_CONFIG]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LOAD_GAME_CONFIG]
GO

----------------------------------------------------------------------------------------------------

-- 插入头像
CREATE PROC GSP_GP_PublicNotic
	@strKeyName NVARCHAR(32),					-- 关键字
	@strErrorDescribe NVARCHAR(512) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 自定义数据
	SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo WHERE StatusName=@strKeyName
	IF @strErrorDescribe IS NULL 
	BEGIN
		SET @strErrorDescribe = N'未找到关键字！'
		return 4		
	END

END	

RETURN 0


GO

----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

-- 加载配置
CREATE PROC GSP_GR_LOAD_GAME_CONFIG
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	DECLARE @DownLoadHttp NVARCHAR(128)
	DECLARE @dwVersion int
	-- 自定义数据
	SELECT @DownLoadHttp=StatusString,@dwVersion=StatusValue FROM SystemStatusInfo WHERE StatusName=N'DownLoadHttp'
	IF @DownLoadHttp IS NULL 
	BEGIN
		SET @DownLoadHttp = N''
	END
	SELECT @DownLoadHttp AS DownLoadHttp,@dwVersion AS Version
END	

RETURN 0


GO