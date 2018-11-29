USE [QPTreasureDB]
GO
/****** Object:  Trigger [dbo].[Tri_OnLineOrder_Update]    Script Date: 2017/8/24 21:21:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER [dbo].[Tri_OnLineOrder_Update] 
ON [dbo].[OnLineOrder] 
AFTER UPDATE 
AS 
BEGIN
	
	IF UPDATE ([OrderStatus])
	BEGIN

	DECLARE @OrderID NVARCHAR(32)
	DECLARE @Statu INT

	SELECT @OrderID = OrderID, @Statu = OrderStatus FROM INSERTED
	
	UPDATE QPAgent.dbo.AgentRecharge SET Statu = @Statu Where OrderID = @OrderID
	END
END