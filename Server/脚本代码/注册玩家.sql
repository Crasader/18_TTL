USE [QPAccountsDB]
GO

DECLARE	@return_value int,
		@strErrorDescribe nvarchar(127)

EXEC	@return_value = [dbo].[GSP_GP_RegisterAccounts]
		@strAccounts = N'test010',
		@strNickName = N'test010',
		@strSpreader = N'',
		@strLogonPass = N'111111',
		@strInsurePass = N'111111',
		@wFaceID = 1,
		@cbGender = 1,
		@strPassPortID = N'test010',
		@strCompellation = N'test010',
		@strClientIP = N'127.0.0.1',
		@strMachineID = N'0',
		@strErrorDescribe = @strErrorDescribe OUTPUT

SELECT	@strErrorDescribe as N'@strErrorDescribe'

SELECT	'Return Value' = @return_value

GO
