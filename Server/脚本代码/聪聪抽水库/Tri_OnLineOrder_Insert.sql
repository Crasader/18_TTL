USE [QPTreasureDB]
GO
/****** Object:  Trigger [dbo].[Tri_OnLineOrder_Insert]    Script Date: 2017/8/24 21:21:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER [dbo].[Tri_OnLineOrder_Insert] 
ON [dbo].[OnLineOrder] 
AFTER INSERT 
AS 
BEGIN
	DECLARE @UserID INT
	DECLARE @OrderID NVARCHAR(32)
	DECLARE @Amount DECIMAL(18, 2)
	DECLARE @Score BIGINT
	DECLARE @RDatetime DATETIME
	DECLARE @Statu INT
	DECLARE @Note NVARCHAR(MAX)

	SELECT @UserID = UserID, @OrderID = OrderID, @Amount = PayAmount, @Score = OrderAmount, @RDatetime = ApplyDate, @Statu = OrderStatus FROM INSERTED
	SET @Note = ''
	
	INSERT INTO QPAgent.dbo.AgentRecharge VALUES(@UserID, @OrderID, @Amount, @Score, @RDatetime, @Statu, @Note)
END