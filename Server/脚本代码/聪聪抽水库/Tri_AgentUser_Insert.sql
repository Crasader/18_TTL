USE [QPAgent]
GO
/****** Object:  Trigger [dbo].[Tri_AgentUser_Insert]    Script Date: 2017/8/24 21:22:03 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER [dbo].[Tri_AgentUser_Insert] 
ON [dbo].[AgentUser] 
AFTER INSERT 
AS
BEGIN
	DECLARE @AgentID INT
	DECLARE @Role NVARCHAR(MAX)
	DECLARE @IsAdmin INT
	DECLARE @Identity NVARCHAR(50)

	SET @Role = 'PT'
	SET @IsAdmin = 0
	SET @Identity = 'Members'
	SELECT @AgentID = AgentID FROM inserted

	INSERT INTO AgentRole VALUES(@AgentID, @Role, @IsAdmin, @Identity)

END