USE [QPTreasureDB]
GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_Pumping]    Script Date: 2017/8/31 20:14:16 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROC [dbo].[GSP_GP_Pumping]
(
	@UserIDs NVARCHAR(200),
	@BottomNote DECIMAL(18, 2),
	@ServerID INT
)
AS
	BEGIN
		DECLARE @BeforeMoney DECIMAL(18, 2)
		DECLARE @Pumping DECIMAL(18, 2)
		DECLARE @RMBExchangeRate INT
		DECLARE @Split NCHAR(1)

		DECLARE @DistributionLevels INT
		DECLARE @i INT
		DECLARE @DistributionRatio DECIMAL(18,2)
		DECLARE @DistributionUpper INT
		DECLARE @AgentIDs NVARCHAR(MAX)
		
		DECLARE @UserID INT
		DECLARE @AgentID INT
		DECLARE @TempAgentID INT
		DECLARE @Proportions NVARCHAR(MAX)

		SELECT @Pumping = CONVERT(DECIMAL(18, 2), CONVERT(DECIMAL(18, 2), a.StatusValue) / 100) FROM QPAccountsDB.dbo.SystemStatusInfo a Where StatusName = 'Game' + Convert(NVARCHAR(5),@ServerID)
		SELECT @RMBExchangeRate = CONVERT(int, a.StatusValue) FROM QPAccountsDB.dbo.SystemStatusInfo a Where StatusName = 'RMBExchangeRate'
		SET @Split = ','
		SET @Proportions = ''
		SET @AgentIDs = ''
		DECLARE CURSOR_UserID CURSOR FOR SELECT * FROM QPAgent.dbo.f_splitSTR(@UserIDs,@Split)
		OPEN CURSOR_UserID
		FETCH NEXT FROM CURSOR_UserID INTO @UserID
		WHILE @@FETCH_STATUS=0
		BEGIN
			SELECT @DistributionLevels = MAX(DistributionLevel) FROM QPAgent.dbo.AgentDistributionLevel(NOLOCK)
			SET @i = 1
			SELECT @AgentID = SpreaderID FROM QPAccountsDB.dbo.AccountsInfo(NOLOCK) Where UserID = @UserID AND SpreaderID <> 0
			IF EXISTS(SELECT AgentID FROM QPAgent.dbo.AgentUser Where UserID = @UserID)
			BEGIN
				SELECT @AgentID = AgentID FROM QPAgent.dbo.AgentUser Where UserID = @UserID
			END
			WHILE @i <= @DistributionLevels
			BEGIN
				SELECT @BeforeMoney = RoomCard FROM QPAgent.dbo.AgentUser Where AgentID = @AgentID and Nullity = 0
				SELECT @DistributionRatio = DistributionRatio, @DistributionUpper = DistributionUpper FROM QPAgent.dbo.AgentDistributionLevel(NOLOCK) Where DistributionLevel = @i
				IF @AgentID IS NOT NULL
				BEGIN
					SET @AgentIDs = @AgentIDs + CONVERT(NVARCHAR(10), @AgentID) + ','
					SET @Proportions = @Proportions + CONVERT(NVARCHAR(10), @DistributionRatio) + ','
					UPDATE QPAgent.dbo.AgentUser SET RoomCard = RoomCard + ROUND(@DistributionRatio * @BottomNote * @Pumping, 2) Where AgentID = @AgentID
				END
				SET @TempAgentID = @AgentID
				SELECT @AgentID = Superior FROM QPAgent.dbo.AgentUser(NOLOCK) Where AgentID = @AgentID and Nullity = 0
				IF @TempAgentID IS NOT NULL
				BEGIN
					INSERT INTO QPAgent.dbo.AgentDistribution VALUES(@i, ISNULL(@AgentID,0), @TempAgentID, @BeforeMoney, ROUND(@DistributionRatio * @BottomNote * @Pumping, 2), @DistributionRatio, GETDATE())
				END
				SET @i = @i + 1
			END
			FETCH NEXT FROM CURSOR_UserID INTO @UserID
		END
		CLOSE CURSOR_UserID
		DEALLOCATE CURSOR_UserID

		IF @AgentIDs <> '' AND @Proportions <> ''
		BEGIN
			SET @AgentIDs = SUBSTRING(@AgentIDs, 0, LEN(@AgentIDs))
			SET @Proportions = SUBSTRING(@Proportions, 0, LEN(@Proportions))
			SELECT ISNULL(@AgentIDs, 'a'), ISNULL(@Proportions, 'b')
			INSERT INTO QPAgent.dbo.AgentUserGame VALUES(@UserIDs, @AgentIDs, @BottomNote, @Pumping, @Proportions, GETDATE())
		END
	END