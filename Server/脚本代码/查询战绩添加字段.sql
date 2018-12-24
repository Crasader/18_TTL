USE QPTreasureDB
GO
/*
ALTER TABLE [dbo].RecordDrawInfo DROP COLUMN BaseScore
ALTER TABLE [dbo].RecordDrawInfo ADD BaseScore BIGINT NOT NULL default 0

ALTER TABLE [dbo].RecordDrawInfo DROP COLUMN RulesBytes
ALTER TABLE [dbo].RecordDrawInfo ADD RulesBytes BIGINT NOT NULL default 0

ALTER TABLE [dbo].RecordDrawInfo DROP COLUMN InSertTime
ALTER TABLE [dbo].RecordDrawInfo ADD InSertTime BIGINT NOT NULL default 0

ALTER TABLE [dbo].RecordDrawInfo DROP COLUMN StartTime
ALTER TABLE [dbo].RecordDrawInfo ADD StartTime BIGINT NOT NULL default 0

ALTER TABLE [dbo].RecordDrawInfo DROP COLUMN ConcludeTime
ALTER TABLE [dbo].RecordDrawInfo ADD ConcludeTime BIGINT NOT NULL default 0

ALTER TABLE [dbo].RecordDrawScore DROP COLUMN InSertTime
ALTER TABLE [dbo].RecordDrawScore ADD InSertTime BIGINT NOT NULL default 0

*/

ALTER TABLE [dbo].RecordDrawScore ADD StartTime BIGINT NOT NULL default 0

GO
