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
-- Create date: <2017��9��11��10:38:49>
-- Description:	<�����ƹ�Ա>
-- =============================================
CREATE PROCEDURE GSP_GP_AddPopularize
	(
		@uid bigint,
		@name varchar(20)
	)
AS
BEGIN
	SET NOCOUNT ON;

	INSERT INTO Popularize(uid,name,code) VALUES (@uid,@name,'0000');
END
GO
