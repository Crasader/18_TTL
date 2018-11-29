USE [QPRecordDB]
GO
/****** Object:  Table [dbo].[RecordWinLosePerRound]    Script Date: 2016/8/22 11:02:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

--局数ID	房间编号	玩家id   	时间		游戏类型	开局金币	本局输赢金币 
CREATE TABLE [dbo].[RecordWinLosePerRound](
	[roundId] [int] IDENTITY(1,1) NOT NULL,
	[roomId] [int] NOT NULL,
	[userId] [int] NOT NULL,
	[endTime] [datetime] NOT NULL,
	[gameType] [int] NOT NULL,
	[beginGold] [int] NOT NULL,
	[winloseGold] [int] NOT NULL,
 CONSTRAINT [roundId] UNIQUE NONCLUSTERED 
(
	[roundId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
