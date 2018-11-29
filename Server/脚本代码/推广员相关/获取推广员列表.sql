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
USE 
	[QPTreasureDB]
GO
-- =============================================
-- Author:		<ttd>
-- Create date: <2017年9月11日10:38:49>
-- Description:	<新增推广员>
-- =============================================
CREATE PROCEDURE GSP_GP_GetPopularizeList
	(
		@type int,
		@size int
	)
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @strSql varchar(4000)

	SET @strSql = 'SELECT TOP' + STR(@size) + '* FROM [QPTreasureDB].[dbo].[Popularize]';
	EXEC(@strSql)
END
GO
