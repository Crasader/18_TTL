USE QPPlatformDB
GO

-- «©µΩ≈‰÷√
TRUNCATE TABLE RouletteConfig
GO

INSERT INTO [dbo].[RouletteConfig] ([RewardGold], [RewardType],[RouletteIdex],[AwardPercent]) VALUES (500, 1,3,50)
INSERT INTO [dbo].[RouletteConfig] ([RewardGold], [RewardType],[RouletteIdex],[AwardPercent]) VALUES (2000,1,6,50)
INSERT INTO [dbo].[RouletteConfig] ([RewardGold], [RewardType],[RouletteIdex],[AwardPercent]) VALUES (1, 2,1,50)
INSERT INTO [dbo].[RouletteConfig] ([RewardGold], [RewardType],[RouletteIdex],[AwardPercent]) VALUES (2, 2,4,50)

GO