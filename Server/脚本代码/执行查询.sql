USE [QPTreasureDB]
GO

--UPDATE [dbo].RecordDrawScore SET UserID = 5 WHERE DrawID = 249 OR DrawID = 250 AND UserID = 10

--SELECT TOP 500  *--DrawID, KindID, TableID, StartTime, ConcludeTime, BaseScore, RulesBytes 
--FROM [dbo].RecordDrawInfo
--ORDER BY InsertTime DESC

--TRUNCATE table [dbo].RecordDrawInfo
--TRUNCATE table [dbo].RecordDrawScore

UPDATE [dbo].RecordDrawInfo SET GameType = 3

GO
