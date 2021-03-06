USE master
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPAgent')
DROP DATABASE [QPAgent]
GO

USE [master]
GO
/****** Object:  Database [QPAgent]    Script Date: 2017/8/31 16:16:41 ******/
CREATE DATABASE [QPAgent]
	CONTAINMENT = NONE
	ON  PRIMARY 
( NAME = N'QPAgent', FILENAME = N'D:\数据库\QPAgent.mdf' , SIZE = 12288KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'QPAgent_log', FILENAME = N'D:\数据库\QPAgent_log.ldf' , SIZE = 26816KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO

ALTER DATABASE [QPAgent] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [QPAgent].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [QPAgent] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [QPAgent] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [QPAgent] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [QPAgent] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [QPAgent] SET ARITHABORT OFF 
GO
ALTER DATABASE [QPAgent] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [QPAgent] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [QPAgent] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [QPAgent] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [QPAgent] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [QPAgent] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [QPAgent] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [QPAgent] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [QPAgent] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [QPAgent] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [QPAgent] SET  DISABLE_BROKER 
GO
ALTER DATABASE [QPAgent] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [QPAgent] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [QPAgent] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [QPAgent] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [QPAgent] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [QPAgent] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [QPAgent] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [QPAgent] SET RECOVERY FULL 
GO
ALTER DATABASE [QPAgent] SET  MULTI_USER 
GO
ALTER DATABASE [QPAgent] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [QPAgent] SET DB_CHAINING OFF 
GO
ALTER DATABASE [QPAgent] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [QPAgent] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
EXEC sys.sp_db_vardecimal_storage_format N'QPAgent', N'ON'
GO
USE [QPAgent]
GO
/****** Object:  StoredProcedure [dbo].[GetInfo]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GetInfo]
	AS
	BEGIN
		DECLARE @Users BIGINT
		DECLARE @Agents BIGINT
		DECLARE @OnLine BIGINT

		DECLARE @ZRebate BIGINT
		DECLARE @ZDistribution BIGINT
		DECLARE @ZMoney BIGINT
		DECLARE @ZRecharge BIGINT

		SELECT @Users = COUNT(1) FROM QPAccountsDB.dbo.AccountsInfo
		SELECT @Agents = COUNT(1) FROM QPAgent.dbo.AgentUser
		SELECT @OnLine = COUNT(1) FROM QPTreasureDB.dbo.GameScoreLocker a inner join QPAccountsDB.dbo.AccountsInfo b on b.UserID = a.UserID inner join QPPlatformDB.dbo.GameKindItem c on c.KindID = a.KindID inner join QPPlatformDB.dbo.GameRoomInfo d on d.ServerID = a.ServerID

		SELECT @ZRebate = SUM(DistributionMoney) FROM QPAgent.dbo.AgentRebate
		SELECT @ZDistribution = SUM(DistributionMoney) FROM QPAgent.dbo.AgentDistribution
		SELECT @ZMoney = SUM([Money]) FROM QPAgent.dbo.AgentUser
		SELECT @ZRecharge = SUM(PayAmount) FROM QPTreasureDB.dbo.OnLineOrder

		SELECT @Users AS 平台用户总数, @Agents AS 平台代理商总数, @OnLine AS 在线总人数
		SELECT @ZRebate AS 代理商充值总返点, @ZDistribution AS 用户充值总返点, @ZMoney AS 平台代理商金钱总额, @ZRecharge AS 平台用户充值总额
	END

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_AddWithdrawal]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_AddWithdrawal]
(
	@dwAgentID int,
	@dwMoney int,
	@dwNote nvarchar(max),
	@strErrorDescribe nvarchar(127) output,
	@dwNum int output
)
	AS
		BEGIN
			DECLARE @AgentID INT
			DECLARE @Money DECIMAL(18,2)

			SELECT @AgentID = AgentID, @Money = [Money] FROM QPAgent..AgentUser(NOLOCK) Where AgentID = @dwAgentID

			IF @AgentID IS NULL
			BEGIN
				SET @strErrorDescribe = '未查询到该用户ID！'
				SET @dwNum = 1
				RETURN
			END

			IF @dwMoney > @Money
			BEGIN
				SET @strErrorDescribe = '提现金额超出用户所有的！'
				SET @dwNum = 2
				RETURN
			END

			IF @dwMoney <= 0
			BEGIN
				SET @strErrorDescribe = '提现金额非法！'
				SET @dwNum = 3
				RETURN
			END

			IF Datepart(weekday, CONVERT(Datetime, GETDATE()) + @@DateFirst - 1) > 5
			BEGIN
				SET @strErrorDescribe = '请于工作日提现！'
				SET @dwNum = 4
				RETURN
			END

			IF DATEDIFF(Day, (SELECT TOP 1 ApplyDate FROM QPAgent.dbo.AgentWithdrawal Where AgentID = @AgentID Order by ApplyDate DESC), GETDATE()) <= 0
			BEGIN
				SET @strErrorDescribe = '每个工作日只能提现一次！'
				SET @dwNum = 5
				RETURN
			END

			UPDATE QPAgent..AgentUser SET [Money] = [Money] - @dwMoney Where AgentID = @AgentID and [Money] - @dwMoney >= 0
			INSERT INTO QPAgent.dbo.AgentWithdrawal VALUES(@AgentID, @dwMoney, 0, @dwNote, GETDATE())

			SET @strErrorDescribe = '提现申请已提交！'
			SET @dwNum = 0
		RETURN
		END

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_AgentPay]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_AgentPay]
(
	@dwAgentid int,
	@dwRagentid int,
	@dwInsureScore int,
	@strClientIP nvarchar(15),
	@strMachineID nvarchar(32),
	@strErrorDescribe nvarchar(127) output,
	@dwNum int output
)
	AS
		SET NOCOUNT ON
		BEGIN
			DECLARE @RoomCard decimal(18,2)
			DECLARE @RroomCard decimal(18,2)
			DECLARE @RMoney decimal(18,2)
			DECLARE @Statu int

			DECLARE @AgentID INT
			DECLARE @DistributionLevels INT
			DECLARE @DistributionRatio DECIMAL(18,2)
			DECLARE @DistributionUpper INT
			DECLARE @AccumulativeDistributionMoney DECIMAL(18, 2)

			SELECT @RoomCard = RoomCard, @RMoney = [Money] FROM QPAgent.dbo.AgentUser(NOLOCK) Where AgentID = @dwAgentid
			SELECT @RroomCard = RoomCard, @Statu = Nullity FROM QPAgent.dbo.AgentUser(NOLOCK) Where AgentID = @dwRagentid

			IF @RoomCard < convert(decimal(18, 2),@dwInsureScore)
			BEGIN
				SET @strErrorDescribe=N'代理商的房卡不够，请向管理员联系充值！'
				SET @dwNum = 4
				RETURN
			END

			IF @Statu IS NULL
			BEGIN
				SET @strErrorDescribe=N'未查询到该代理商ID对应的用户，请检查代理商ID！'
				SET @dwNum = 1
				RETURN
			RETURN
			END

			IF @Statu <> 0
			BEGIN
				SET @strErrorDescribe=N'该代理商账号未处于正常状态！'
				SET @dwNum = 2
				RETURN
			RETURN
			END

			BEGIN TRY
				BEGIN TransAction
					DECLARE @OrderID NVARCHAR(32)
					SET @OrderID = CONVERT(varchar(100), GETDATE(), 12) + CONVERT(nvarchar(6),@dwAgentid) + REPLACE(CONVERT(varchar(100), GETDATE(), 10),'-','')
					INSERT INTO QPAgent.dbo.AgentPay VALUES(@dwAgentid, @dwRagentid,@OrderID, 0, @RroomCard, 0, '', 2, GETDATE(), @strClientIP, @strMachineID, '')
					UPDATE QPAgent.dbo.AgentUser SET RoomCard = RoomCard + @dwInsureScore Where AgentID = @dwRagentid
					UPDATE QPAgent.dbo.AgentUser SET RoomCard = RoomCard - @dwInsureScore Where AgentID = @dwAgentid

					DECLARE @i INT
					DECLARE @Money INT
					SELECT @DistributionLevels = MAX(DistributionLevel) FROM AgentRebateLevel(NOLOCK)
					SELECT @Money = Score FROM QPAgent.dbo.AgentRechargeOption Where RoomCardCount = @dwInsureScore
					SET @i = 1
					SET @AgentID = @dwRagentid
					WHILE @i <= @DistributionLevels
					BEGIN
						SELECT @AgentID = Superior FROM AgentUser Where AgentID = @AgentID
						SELECT @DistributionRatio = DistributionRatio, @DistributionUpper = DistributionUpper FROM AgentRebateLevel(NOLOCK) Where DistributionLevel = @i
						IF @dwRagentid IS NOT NULL
						BEGIN
							SELECT @RoomCard = RoomCard FROM AgentUser(NOLOCK) Where AgentID = @AgentID
							SELECT @AccumulativeDistributionMoney = SUM(DistributionMoney) FROM AgentDistribution Where @AgentID = @dwRagentid and DistributionLevel = @i
							IF @AccumulativeDistributionMoney IS NULL
							BEGIN
								SET @AccumulativeDistributionMoney = 0
							END
							IF @DistributionRatio IS NOT NULL and @DistributionUpper >= @AccumulativeDistributionMoney + (@DistributionRatio * @Money) and @DistributionRatio != 0
							BEGIN
								UPDATE AgentUser SET [Money] = [Money] + (@DistributionRatio * @Money) Where AgentID = @AgentID
								INSERT INTO AgentRebate VALUES(@AgentID, @dwRagentid, @i, @RMoney, (@DistributionRatio * @Money), @DistributionRatio, GETDATE())
							END
						END
						ELSE
						RETURN

						SET @i = @i + 1
					END

					SET @strErrorDescribe = '充值完成！'
					SET @dwNum = 0
				Commit TransAction
			END TRY
			BEGIN CATCH
				ROLLBACK TransAction
			END CATCH
		END

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_AlterPassword]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_AlterPassword]
(
	@dwAgentID int,
	@dwDQAgentID int,
	@stroldPassword nvarchar(32),
	@strnewPassword nvarchar(32),
	@strtwoPassword nvarchar(32),
	@strErrorDescribe nvarchar(127) output,
	@dwNum int output
)
	AS
		BEGIN
			DECLARE @pass nvarchar(32)
			DECLARE @Admin int

			SELECT @pass = LogonPass FROM QPAccountsDB.dbo.AccountsInfo Where UserID = @dwAgentID
			SELECT @Admin = IsAdmin FROM QPAgent.dbo.AgentRole Where AgentID = @dwDQAgentID

			IF @Admin = 1
			BEGIN
				DECLARE @adminpass nvarchar(32)
				SELECT @adminpass = ManagePassword FROM QPAgent.dbo.AgentConfig

				IF @strtwoPassword <> @adminpass
				BEGIN
					SET @strErrorDescribe = '管理员二级密码不匹配！'
					SET @dwNum = 2
					RETURN
				END
			END

			ELSE
			BEGIN
				IF @pass <> @stroldPassword
				BEGIN
					SET @strErrorDescribe = '填写旧密码不匹配！'
					SET @dwNum = 1
					RETURN
				END
			END

			UPDATE QPAgent.dbo.AgentUser SET LogonPass = @strnewPassword Where AgentID = @dwAgentID
			SET @strErrorDescribe = '密码已修改，请重新登录！'
			SET @dwNum = 0
		RETURN
		END
RETURN 0

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_EfficacyAccounts]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_EfficacyAccounts]
(
	@strAccounts NVARCHAR(31),					-- 用户帐号
	@strPassword NCHAR(32),						-- 用户密码
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
	@nNeeValidateMBCard BIT,					-- 密保校验
	@strErrorDescribe NVARCHAR(127) OUTPUT,		-- 输出信息
	@dwNum INT OUTPUT							-- 输出参数
)
	AS
	
-- 执行逻辑
BEGIN
 
	-- 查询用户
	DECLARE @Nullity int
	DECLARE @LogonPass nvarchar(32)
	DECLARE @AgentID int
	SELECT @Nullity=Nullity, @LogonPass = LogonPass, @AgentID = AgentID FROM QPAgent.dbo.AgentUser (NOLOCK) WHERE Accounts = @strAccounts

	-- 密码判断
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		SET @dwNum = 1
		RETURN
	END
	-- 查询用户
	IF @AgentID IS NULL
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		SET @dwNum = 2
		RETURN
	END

	ELSE
	BEGIN
		IF @Nullity = 1
		BEGIN
			SET @strErrorDescribe=N'您的帐号已被禁用！'
		SET @dwNum = 3
		RETURN
		END
	END
	
	--插入登录登录记录
	INSERT INTO QPAgent.dbo.AgentLogon_log VALUES(@AgentID, @strClientIP, @strMachineID, GETDATE(), 1)
	SET @dwNum = 0
		RETURN
END


/****** Object:  StoredProcedure [dbo].[WEB_PageView]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[WEB_PageView]
(
	@TableName		NVARCHAR(200),			-- 表名
	@ReturnFields	NVARCHAR(1000) = '*',	-- 查询列数
	@PageSize		INT = 10,				-- 每页数目
	@PageIndex		INT = 1,				-- 当前页码
	@Where			NVARCHAR(MAX) = '',	-- 查询条件
	@OrderBy		NVARCHAR(1000)			-- 排序字段
)
AS
--设置属性
SET NOCOUNT ON

-- 变量定义
DECLARE @TotalRecord INT
DECLARE @TotalPage INT
DECLARE @CurrentPageSize INT
DECLARE @TotalRecordForPageIndex INT
DECLARE @RecordCount INT
DECLARE @PageCount INT

BEGIN
	SET @RecordCount = NULL
	SET @PageCount = NULL

	IF @Where IS NULL SET @Where=N''
	
	-- 记录总数
	DECLARE @countSql NVARCHAR(4000)  
	
	IF @RecordCount IS NULL
	BEGIN
		SET @countSql='SELECT @TotalRecord=Count(*) From '+@TableName+' '+@Where
		print @countSql
		EXECUTE sp_executesql @countSql,N'@TotalRecord int out',@TotalRecord OUT
	END
	ELSE
	BEGIN
		SET @TotalRecord=@RecordCount
	END		
	
	SET @RecordCount=@TotalRecord
	SET @TotalPage=(@TotalRecord-1)/@PageSize+1	
	SET @CurrentPageSize=(@PageIndex-1)*@PageSize

	-- 返回总页数和总记录数
	SET @PageCount=@TotalPage
	SET @RecordCount=@TotalRecord

	SELECT @PageCount AS PageCounts, @RecordCount AS RecordCount
		
	-- 返回记录
	SET @TotalRecordForPageIndex=@PageIndex*@PageSize
	
	EXEC	('SELECT *
			FROM (SELECT TOP '+@TotalRecordForPageIndex+' ROW_NUMBER() OVER ('+@OrderBy+') AS #, '+@ReturnFields+'  FROM '+@TableName+ ' ' + @Where +' ) AS TempPageViewTable WHERE TempPageViewTable.# >  '+@CurrentPageSize)
	
END
RETURN 0

GO





GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_GetSidebar]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_GetSidebar]
(
	@dwAgentID int
)
	AS
		BEGIN
			DECLARE @Role NVARCHAR(max)

			SELECT @Role = [Role] FROM AgentRole Where AgentID = @dwAgentID

			DECLARE Cursor_Sidebar CURSOR FOR SELECT ID FROM [QPAgent].[dbo].[AgentSidebar] Where NavIcon != '' and Nullity = 0 and CHARINDEX(@Role, NavRole) > 0 Order by Sort
			OPEN Cursor_Sidebar
			DECLARE @ID INT
			WHILE @@FETCH_STATUS=0
			BEGIN
				SELECT NavIcon, NavName, NavUrl FROM [QPAgent].[dbo].[AgentSidebar] Where NavSuperiorID = @ID and Nullity = 0 and CHARINDEX(@Role, NavRole) > 0 Order by NavIcon desc
				FETCH NEXT FROM Cursor_Sidebar INTO @ID
			END
		END
		CLOSE Cursor_Sidebar
			DEALLOCATE Cursor_Sidebar


GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_Pumping]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_Pumping]
(
	@UserIDs NVARCHAR(200),
	@BottomNote DECIMAL(18, 2)
)
AS
	BEGIN
		DECLARE @Pumping DECIMAL(18, 2)
		DECLARE @Split NCHAR(1)

		DECLARE @DistributionLevels INT
		DECLARE @i INT
		DECLARE @DistributionRatio DECIMAL(18,2)
		DECLARE @DistributionUpper INT
		DECLARE @AgentIDs NVARCHAR(200)
		
		DECLARE @UserID INT
		DECLARE @AgentID INT
		DECLARE @Proportions NVARCHAR(200)

		SELECT @Pumping = a.StatusValue / 100 FROM QPAccountsDB.dbo.SystemStatusInfo a Where StatusName = 'Pumping'
		SET @Split = ','
		
		DECLARE CURSOR_UserID CURSOR FOR SELECT * FROM QPAgent.dbo.f_splitSTR(@UserIDs,@Split)
		OPEN CURSOR_UserID
		
		WHILE @@FETCH_STATUS=0
		BEGIN
			FETCH LAST FROM CURSOR_UserID INTO @UserID
			SELECT @DistributionLevels = MAX(DistributionLevel) FROM QPAgent.dbo.AgentDistributionLevel(NOLOCK)
			SET @i = 1
			SELECT @AgentID = SpreaderID FROM QPAccountsDB.dbo.AccountsInfo Where UserID = @UserID AND SpreaderID <> 0
			WHILE @i <= @DistributionLevels
			BEGIN
				SELECT @DistributionRatio = DistributionRatio, @DistributionUpper = DistributionUpper FROM QPAgent.dbo.AgentDistributionLevel(NOLOCK) Where DistributionLevel = @i
				IF @AgentID IS NOT NULL
				BEGIN
					SET @AgentIDs += @AgentID + @Split
					SET @Proportions += @DistributionRatio + @Split
					UPDATE QPAgent.dbo.AgentUser SET RoomCard = ROUND(@DistributionRatio * @BottomNote, 2)
				END
				SELECT @AgentID = Superior FROM QPAgent.dbo.AgentUser Where AgentID = @AgentID and Nullity = 0
			END
		END
		CLOSE CURSOR_UserID
		DEALLOCATE CURSOR_UserID

		IF @AgentIDs IS NOT NULL
		BEGIN
			SET @AgentIDs = SUBSTRING(@AgentIDs, 0, LEN(@AgentIDs) - 1)
			SET @Proportions = SUBSTRING(@Proportions, 0, LEN(@Proportions) - 1)
			INSERT INTO QPAgent.dbo.AgentUserGame VALUES(@UserIDs, @AgentIDs, @BottomNote, @Pumping, @Proportions, GETDATE())
		END
	END

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_RechargeInsureScore]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_RechargeInsureScore]
(
	@dwAgentid int,
	@dwUserid int,
	@dwMoney int,
	@dwInsureScore int,
	@strClientIP nvarchar(15),
	@strMachineID nvarchar(32),
	@strErrorDescribe nvarchar(127) output,
	@dwNum int output
)
	AS
		DECLARE @RoomCard decimal(18,2)
		DECLARE @GameID int
		DECLARE @Accounts nvarchar(32)
		DECLARE @SpreaderID int
		DECLARE @Nullity int
		DECLARE @OrderID nvarchar(32)
		DECLARE @InsureScore bigint
		DECLARE @IsAdmin int

		SELECT @RoomCard = RoomCard FROM QPAgent.dbo.AgentUser(NOLOCK) Where AgentID = @dwAgentid
		SELECT @Accounts = Accounts, @GameID = GameID, @SpreaderID = SpreaderID, @Nullity = Nullity FROM QPAccountsDB.dbo.AccountsInfo(NOLOCK) a Where a.UserID = @dwUserid
		SELECT @InsureScore = SUM(Score) FROM QPTreasureDB.dbo.GameScoreInfo(NOLOCK) Where UserID = @dwUserid
		SELECT @IsAdmin = IsAdmin FROM QPAgent.dbo.AgentRole(NOLOCK) Where AgentID = @dwAgentid

		SELECT @SpreaderID, @dwAgentid, @IsAdmin, @Nullity

		IF @RoomCard < convert(decimal(18, 2),@dwInsureScore)
		BEGIN
			SET @strErrorDescribe=N'代理商的金币不够，请向管理员联系充值！'
			SET @dwNum = 4
			RETURN
		END

		IF @Accounts IS NULL
		BEGIN
			SET @strErrorDescribe=N'未查询到该用户ID对应的用户，请检查用户ID！'
			SET @dwNum = 1
			RETURN
		RETURN
		END

		IF @SpreaderID <> @dwAgentid
		BEGIN
			IF @IsAdmin = 0
			BEGIN
				SET @strErrorDescribe=N'该用户代理商不属于当前代理商！'
				SET @dwNum = 3
				RETURN
			END
		END

		IF @Nullity<>0
		BEGIN
			SET @strErrorDescribe=N'该用户账号未处于正常状态！'
			SET @dwNum = 2
			RETURN
		END
		BEGIN TRY
		BEGIN TransAction
			SET @OrderID = CONVERT(varchar(100), GETDATE(), 12) + CONVERT(nvarchar(6),@dwUserid) + REPLACE(CONVERT(varchar(100), GETDATE(), 10),'-','')
			
			SELECT @dwUserid, 2,@dwUserid, @GameID, @Accounts, @OrderID, CONVERT(decimal(18,2),@dwMoney), 1, CONVERT(decimal(18,2),@dwMoney), 1, @dwInsureScore, 2, @strClientIP, GETDATE() 
			PRINT 1
			INSERT INTO QPTreasureDB.dbo.OnLineOrder VALUES(@dwUserid, 2, @dwUserid, @GameID, @Accounts, @OrderID, 0,0,0,0,CONVERT(decimal(18,2),@dwMoney), 1, CONVERT(decimal(18,2),@dwMoney), 2, @strClientIP, GETDATE())
			INSERT INTO QPTreasureDB.dbo.ShareDetailInfo VALUES(@dwUserid, 2, @dwUserid, @GameID, @Accounts, '', @OrderID, 0, 0, 0, @InsureScore, @dwMoney, CONVERT(decimal(18,2),@dwMoney), 1, CONVERT(decimal(18,2),@dwMoney), @strClientIP, GETDATE())
			PRINT 2
			IF @IsAdmin = 0
			BEGIN
				UPDATE QPAgent.dbo.AgentUser SET RoomCard = RoomCard - @dwInsureScore Where AgentID = @dwAgentid
			END

			SET ROWCOUNT 1 UPDATE QPTreasureDB.dbo.GameScoreInfo SET Score += @dwInsureScore Where UserID = @dwUserid

			SET @strErrorDescribe=N'已成功充值！'
			SET @dwNum = 0
		Commit TransAction
		END TRY
		BEGIN CATCH
			ROLLBACK TransAction
		END CATCH

GO
/****** Object:  StoredProcedure [dbo].[GSP_GP_RegisterAccounts]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[GSP_GP_RegisterAccounts]
(
	@UserID int,								-- 用户ID
	@strSpreader NVARCHAR(31),					-- 代理商用户ID
	@dwCity INT,								-- 城市ID
	@ImgCardIDImg NVARCHAR(MAX),						-- 身份证图片
	@ImgSelfImg NVARCHAR(MAX),							-- 本人图片
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
)
	 AS

	-- 属性设置
	SET NOCOUNT ON

	BEGIN
		DECLARE @MobilePhone nvarchar(16)
		DECLARE @Compellation nvarchar(16)
		DECLARE @SpreaderUserID int
		SELECT @SpreaderUserID = UserID FROM QPAccountsDB..AccountsInfo(nolock) Where Accounts = @strSpreader 
		SELECT @MobilePhone = Accounts, @Compellation = Compellation FROM QPAccountsDB..AccountsInfo(nolock) Where UserID = @UserID

		INSERT INTO AgentUser VALUES(@UserID, @SpreaderUserID, 0.00, 0.00, @dwCity, @ImgCardIDImg, @ImgSelfImg, 0)
		INSERT INTO AgentRole VALUES(@UserID, 'PT', 0)
		INSERT INTO AgentLogon_log VALUES(@UserID, @strClientIP, @strMachineID, GETDATE(), 0)

		INSERT INTO QPAccountsDB..IndividualDatum VALUES(@UserID, @Compellation, '', '', '', @MobilePhone, '', '', GETDATE(), '', '', '')

		SET @strErrorDescribe = ''
	END
RETURN 0


GO
/****** Object:  StoredProcedure [dbo].[Query_AgentList]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[Query_AgentList]
(
	@strWhere NVARCHAR(1000),					-- 条件查询
	@strOrderBy NVARCHAR(1000),					-- 排序
	@dwPageSize NVARCHAR(5),					-- 页数显示数量
	@dwPageIndex NVARCHAR(5)					-- 当前页数
)
AS
	BEGIN
		DECLARE @TableName NVARCHAR(200)
		DECLARE @ReturnFields NVARCHAR(1000)
		DECLARE @Where NvARCHAR(1000)
	
		IF @strOrderBy = ''
		BEGIN
			SET @strOrderBy = 'order by a.RegisterDate desc'
		END

		SET @TableName = 'QPAgent.dbo.AgentUser a'
		SET @ReturnFields = 'a.AgentID AS ID, a.AgentID AS 代理商ID,a.Superior AS 上级代理商ID , a.Accounts AS 账号, a.Phone AS 手机号, a.Compellation AS 姓名, a.RegisterDate AS 注册时间, QPAccountsDB.dbo.GetAccountStatu(a.Nullity) AS 账号状态, a.[Money] AS 剩余金额, QPAgent.dbo.GetRechangeCount(a.AgentID) AS 总充值房卡数量'
		SET @Where = 'Where 1=1' + @strWhere

		EXEC QPAgent.dbo.WEB_PageView @TableName, @ReturnFields, @dwPageSize, @dwPageIndex, @Where, @strOrderBy
	END


GO
/****** Object:  StoredProcedure [dbo].[Query_OnlineUsers]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[Query_OnlineUsers]					-- 查询在线用户
(
	@strWhere NVARCHAR(1000),					-- 条件查询
	@strOrderBy NVARCHAR(1000),					-- 排序
	@dwPageSize NVARCHAR(5),					-- 页数显示数量
	@dwPageIndex NVARCHAR(5)					-- 当前页数
)
AS
	BEGIN
		DECLARE @TableName NVARCHAR(200)
		DECLARE @ReturnFields NVARCHAR(1000)
		DECLARE @Where NvARCHAR(1000)

		IF @strOrderBy = ''
		BEGIN
			SET @strOrderBy = 'order by a.CollectDate desc'
		END

		SET @TableName = 'QPTreasureDB.dbo.GameScoreLocker a inner join QPAccountsDB.dbo.AccountsInfo b'
		SET @ReturnFields = 'b.UserID AS ID, b.UserID AS 用户ID, b.Accounts AS 账号, QPAgent.dbo.GetCityNameForSpreaderID(b.SpreaderID) AS 城市, c.KindName AS 游戏名称, d.ServerName AS 房间名称, a.EnterIP AS 用户IP, a.CollectDate AS 进入时间'
		SET @Where = 'on b.UserID = a.UserID inner join QPPlatformDB.dbo.GameKindItem c on c.KindID = a.KindID inner join QPPlatformDB.dbo.GameRoomInfo d on d.ServerID = a.ServerID'

		EXEC QPAgent.dbo.WEB_PageView @TableName, @ReturnFields, @dwPageSize, @dwPageIndex, @Where, @strOrderBy
	END


GO
/****** Object:  StoredProcedure [dbo].[Query_UserList]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROC [dbo].[Query_UserList]						-- 查询用户列表
(
	@strWhere NVARCHAR(1000),					-- 条件查询
	@strOrderBy NVARCHAR(1000),					-- 排序
	@dwPageSize INT,							-- 页数显示数量
	@dwPageIndex INT							-- 当前页数
)
AS
BEGIN
	
	DECLARE @TableName NVARCHAR(200)
	DECLARE @ReturnFields NVARCHAR(1000)
	DECLARE @Where NvARCHAR(1000)
	
	IF @strOrderBy = ''
	BEGIN
		SET @strOrderBy = 'order by a.RegisterDate desc'
	END

	SET @TableName = 'QPAccountsDB.dbo.AccountsInfo a inner join QPTreasureDB.dbo.GameScoreInfo b'
	SET @ReturnFields = 'a.UserID AS ID, a.UserID AS 用户ID, a.SpreaderID AS 代理商ID, a.Accounts AS 账号, a.NickName AS 昵称, b.InsureScore AS 房卡总数, a.RegisterDate AS 注册时间, QPAccountsDB.dbo.GetAccountStatu(a.Nullity) AS 账号状态'
	SET @Where = 'on b.UserID = a.UserID' + @strWhere

	EXEC QPAgent.dbo.WEB_PageView @TableName, @ReturnFields, @dwPageSize, @dwPageIndex, @Where, @strOrderBy
END


GO
/****** Object:  StoredProcedure [dbo].[sp__windbi$decrypt]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[sp__windbi$decrypt]
(
  @procedure SYSNAME = NULL ,
  @revfl INT = 1
)
AS /**//*
王成辉翻译整理，转贴请注明出自微软BI开拓者www.windbi.com
调用形式为：
exec dbo.sp__windbi$decrypt @procedure,0
如果第二个参数使用1的话，会给出该存储过程的一些提示。
--版本4.0  修正存储过程过长解密出来是空白的问题
*/
SET NOCOUNT ON
IF @revfl = 1 
    BEGIN
        PRINT '警告：该存储过程会删除并重建原始的存储过程。'
        PRINT ' 在运行该存储过程之前确保你的数据库有一个备份。'
        PRINT ' 该存储过程通常应该运行在产品环境的一个备份的非产品环境下。'
        PRINT ' 为了运行这个存储过程，将参数@refl的值更改为0。'
        RETURN 0
    END
DECLARE @intProcSpace BIGINT ,
    @t BIGINT ,
    @maxColID SMALLINT ,
    @procNameLength INT
SELECT  @maxColID = MAX(subobjid)
FROM    sys.sysobjvalues
WHERE   objid = OBJECT_ID(@procedure)
--select @maxColID as 'Rows in sys.sysobjvalues'
SELECT  @procNameLength = DATALENGTH(@procedure) + 29
DECLARE @real_01 NVARCHAR(MAX)
DECLARE @fake_01 NVARCHAR(MAX)
DECLARE @fake_encrypt_01 NVARCHAR(MAX)
DECLARE @real_decrypt_01 NVARCHAR(MAX) ,
    @real_decrypt_01a NVARCHAR(MAX)
DECLARE @objtype VARCHAR(2) ,
    @ParentName NVARCHAR(MAX)
SELECT  @real_decrypt_01a = ''
--提取对象的类型如是存储过程还是函数，如果是触发器，还要得到其父对象的名称
SELECT  @objtype = type, @parentname = OBJECT_NAME(parent_object_id)
FROM    sys.objects
WHERE   [object_id] = OBJECT_ID(@procedure)
-- 从sys.sysobjvalues里提出加密的imageval记录
SET @real_01 = ( SELECT TOP 1
                        imageval
                 FROM   sys.sysobjvalues
                 WHERE  objid = OBJECT_ID(@procedure) AND valclass = 1
                 ORDER BY subobjid
               )
--创建一个临时表
CREATE TABLE #output
(
  [ident] [int] IDENTITY(1, 1)
                NOT NULL ,
  [real_decrypt] NVARCHAR(MAX)
)
------------------------------------------------------------

--开始一个事务，稍后回滚
BEGIN TRAN

--更改原始的存储过程，用短横线替换
IF @objtype = 'P' 
    SET @fake_01 = 'ALTER PROCEDURE ' + @procedure + ' WITH ENCRYPTION AS select 1
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
ELSE 
    IF @objtype = 'FN' 
        SET @fake_01 = 'ALTER FUNCTION ' + @procedure + '() RETURNS INT WITH ENCRYPTION AS BEGIN RETURN 1
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/ END'
    ELSE 
        IF @objtype = 'V' 
            SET @fake_01 = 'ALTER view ' + @procedure + ' WITH ENCRYPTION AS select 1 as col
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
        ELSE 
            IF @objtype = 'TR' 
                SET @fake_01 = 'ALTER trigger ' + @procedure + ' ON ' + @parentname + 'WITH ENCRYPTION AFTER INSERT AS RAISERROR (''N'',16,10)
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
EXECUTE (@fake_01)
--从sys.sysobjvalues里提出加密的假的
SET @fake_encrypt_01 = ( SELECT TOP 1
                                imageval
                         FROM   sys.sysobjvalues
                         WHERE  objid = OBJECT_ID(@procedure) AND valclass = 1
                         ORDER BY subobjid
                       )
IF @objtype = 'P' 
    SET @fake_01 = 'Create PROCEDURE ' + @procedure + ' WITH ENCRYPTION AS select 1
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
ELSE 
    IF @objtype = 'FN' 
        SET @fake_01 = 'CREATE FUNCTION ' + @procedure + '() RETURNS INT WITH ENCRYPTION AS BEGIN RETURN 1
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/ END'
    ELSE 
        IF @objtype = 'V' 
            SET @fake_01 = 'Create view ' + @procedure + ' WITH ENCRYPTION AS select 1 as col
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
        ELSE 
            IF @objtype = 'TR' 
                SET @fake_01 = 'Create trigger ' + @procedure + ' ON ' + @parentname + 'WITH ENCRYPTION AFTER INSERT AS RAISERROR (''N'',16,10)
  /**//*' + REPLICATE(CAST('*' AS NVARCHAR(MAX)),
                      DATALENGTH(@real_01) / 2 - @procNameLength) + '*/'
--开始计数
SET @intProcSpace = 1
--使用字符填充临时变量
SET @real_decrypt_01 = REPLICATE(CAST('A' AS NVARCHAR(MAX)),
                                 ( DATALENGTH(@real_01) / 2 ))
--循环设置每一个变量，创建真正的变量
--每次一个字节
SET @intProcSpace = 1
--如有必要，遍历每个@real_xx变量并解密
WHILE @intProcSpace <= ( DATALENGTH(@real_01) / 2 ) 
    BEGIN
--真的和假的和加密的假的进行异或处理
        SET @real_decrypt_01 = STUFF(@real_decrypt_01, @intProcSpace, 1,
                                     NCHAR(UNICODE(SUBSTRING(@real_01,
                                                             @intProcSpace, 1)) ^ ( UNICODE(SUBSTRING(@fake_01,
                                                              @intProcSpace, 1)) ^ UNICODE(SUBSTRING(@fake_encrypt_01,
                                                              @intProcSpace, 1)) )))
        SET @intProcSpace = @intProcSpace + 1
    END
--通过sp_helptext逻辑向表#output里插入变量
INSERT  #output ( real_decrypt )
        SELECT  @real_decrypt_01
--select real_decrypt AS '#output chek' from #output --测试
-- -------------------------------------
--开始从sp_helptext提取
-- -------------------------------------
DECLARE @dbname SYSNAME ,
    @BlankSpaceAdded INT ,
    @BasePos INT ,
    @CurrentPos INT ,
    @TextLength INT ,
    @LineId INT ,
    @AddOnLen INT ,
    @LFCR INT --回车换行的长度
    ,
    @DefinedLength INT ,
    @SyscomText NVARCHAR(MAX) ,
    @Line NVARCHAR(255)
SELECT  @DefinedLength = 255
SELECT  @BlankSpaceAdded = 0 --跟踪行结束的空格。注意Len函数忽略了多余的空格
CREATE TABLE #CommentText
(
  LineId INT ,
  Text NVARCHAR(255) COLLATE database_default
)
--使用#output代替sys.sysobjvalues
DECLARE ms_crs_syscom CURSOR LOCAL
FOR
SELECT  real_decrypt
FROM    #output
ORDER BY ident FOR READ ONLY
--获取文本
SELECT  @LFCR = 2
SELECT  @LineId = 1
OPEN ms_crs_syscom
FETCH NEXT FROM ms_crs_syscom INTO @SyscomText

WHILE @@fetch_status >= 0 
    BEGIN
        SELECT  @BasePos = 1
        SELECT  @CurrentPos = 1
        SELECT  @TextLength = LEN(@SyscomText)
        WHILE @CurrentPos != 0 
            BEGIN
        --通过回车查找行的结束
                SELECT  @CurrentPos = CHARINDEX(CHAR(13) + CHAR(10),
                                                @SyscomText, @BasePos)
        --如果找到回车
                IF @CurrentPos != 0 
                    BEGIN
                --如果@Lines的长度的新值比设置的大就插入@Lines目前的内容并继续
                        WHILE ( ISNULL(LEN(@Line), 0) + @BlankSpaceAdded + @CurrentPos - @BasePos + @LFCR ) > @DefinedLength 
                            BEGIN
                                SELECT  @AddOnLen = @DefinedLength - ( ISNULL(LEN(@Line),
                                                              0) + @BlankSpaceAdded )
                                INSERT  #CommentText
                                VALUES  ( @LineId,
                                          ISNULL(@Line, N'') + ISNULL(SUBSTRING(@SyscomText,
                                                              @BasePos,
                                                              @AddOnLen), N'') )
                                SELECT  @Line = NULL, @LineId = @LineId + 1,
                                        @BasePos = @BasePos + @AddOnLen,
                                        @BlankSpaceAdded = 0
                            END
                        SELECT  @Line = ISNULL(@Line, N'') + ISNULL(SUBSTRING(@SyscomText,
                                                              @BasePos,
                                                              @CurrentPos - @BasePos + @LFCR),
                                                              N'')
                        SELECT  @BasePos = @CurrentPos + 2
                        INSERT  #CommentText
                        VALUES  ( @LineId, @Line )
                        SELECT  @LineId = @LineId + 1
                        SELECT  @Line = NULL
                    END
                ELSE
--如果回车没找到
                    BEGIN
                        IF @BasePos <= @TextLength 
                            BEGIN
                        --如果@Lines长度的新值大于定义的长度
                                WHILE ( ISNULL(LEN(@Line), 0) + @BlankSpaceAdded + @TextLength - @BasePos + 1 ) > @DefinedLength 
                                    BEGIN
                                        SELECT  @AddOnLen = @DefinedLength - ( ISNULL(LEN(@Line),
                                                              0) + @BlankSpaceAdded )
                                        INSERT  #CommentText
                                        VALUES  ( @LineId,
                                                  ISNULL(@Line, N'') + ISNULL(SUBSTRING(@SyscomText,
                                                              @BasePos,
                                                              @AddOnLen), N'') )
                                        SELECT  @Line = NULL,
                                                @LineId = @LineId + 1,
                                                @BasePos = @BasePos + @AddOnLen,
                                                @BlankSpaceAdded = 0
                                    END
                                SELECT  @Line = ISNULL(@Line, N'') + ISNULL(SUBSTRING(@SyscomText,
                                                              @BasePos,
                                                              @TextLength - @BasePos + 1),
                                                              N'')
                                IF LEN(@Line) < @DefinedLength AND CHARINDEX(' ',
                                                              @SyscomText,
                                                              @TextLength + 1) > 0 
                                    BEGIN
                                        SELECT  @Line = @Line + ' ',
                                                @BlankSpaceAdded = 1
                                    END
                            END
                    END
            END
        FETCH NEXT FROM ms_crs_syscom INTO @SyscomText
    END
    
    
IF @Line IS NOT NULL 
    INSERT  #CommentText
    VALUES  ( @LineId, @Line )
SELECT  Text
FROM    #CommentText
ORDER BY LineId
CLOSE ms_crs_syscom
DEALLOCATE ms_crs_syscom
DROP TABLE #CommentText

-- -------------------------------------
--结束从sp_helptext提取
-- -------------------------------------
--删除用短横线创建的存储过程并重建原始的存储过程
ROLLBACK TRAN
DROP TABLE #output

GO

/****** Object:  UserDefinedFunction [dbo].[f_splitSTR]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[f_splitSTR](
@s   varchar(8000),   --待分拆的字符串
@split varchar(10)     --数据分隔符
)RETURNS @re TABLE(col varchar(100))
AS
BEGIN
 DECLARE @splitlen int
 SET @splitlen=LEN(@split+'a')-2
 WHILE CHARINDEX(@split,@s)>0
 BEGIN
  INSERT @re VALUES(LEFT(@s,CHARINDEX(@split,@s)-1))
  SET @s=STUFF(@s,1,CHARINDEX(@split,@s)+@splitlen,'')
 END
 INSERT @re VALUES(@s)
 RETURN
END


GO
/****** Object:  UserDefinedFunction [dbo].[GetCityName]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[GetCityName]
(
	@dwCityID int					-- 城市ID
)
RETURNS NVARCHAR(200)
AS
BEGIN
	DECLARE @Province NVARCHAR(200)
	DECLARE @ProvinceID INT
	DECLARE @City NVARCHAR(200)
	DECLARE @CityID INT
	DECLARE @Region NVARCHAR(200)

	SET @ProvinceID = CONVERT(INT,SUBSTRING(CONVERT(NVARCHAR(50),@dwCityID),1, 2) + '000000')
	SET @CityID = CONVERT(INT,SUBSTRING(CONVERT(NVARCHAR(50),@dwCityID),1, 5) + '000')

	SELECT @Province = Name FROM QPAgent.dbo.AgentCitys Where ID = @ProvinceID
	SELECT @City = Name FROM QPAgent.dbo.AgentCitys Where ID = @CityID
	SELECT @Region = Name FROM QPAgent.dbo.AgentCitys Where ID = @dwCityID

	SET @Region = @Province + '-' + @City + '-' + @Region

	RETURN @Region
END


GO
/****** Object:  UserDefinedFunction [dbo].[GetCityNameForSpreaderID]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE FUNCTION [dbo].[GetCityNameForSpreaderID]
(
	@dwSpreaderID int
)
	RETURNS NVARCHAR(50)
	AS
	BEGIN
		DECLARE @CityID INT
		DECLARE @CityName NVARCHAR(50)

		SELECT @CityID = CityID FROM QPAgent.dbo.AgentUser Where AgentID = @dwSpreaderID
		SELECT @CityName = QPAgent.dbo.GetCityName(@CityID) 
		RETURN @CityName
	END

GO
/****** Object:  UserDefinedFunction [dbo].[GetOrderStatu]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[GetOrderStatu]
(
	@Statu int
)
RETURNS nvarchar(50)
AS
BEGIN
	DECLARE @StatuStr nvarchar(50)

	IF @Statu = 0
	BEGIN
		SET @StatuStr = '未付款'
	END

	ELSE IF @Statu = 1
	BEGIN
		SET @StatuStr = '支付成功'
	END

	ELSE IF @Statu = 2
	BEGIN
		SET @StatuStr = '支付失败'
	END
	
	RETURN @StatuStr
END


GO
/****** Object:  UserDefinedFunction [dbo].[GetRechangeCount]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[GetRechangeCount]
(
	@dwUserID int -- 代理商用户ID
)
	RETURNS BIGINT
	AS
	BEGIN
		DECLARE @Count BIGINT
		SELECT @Count = SUM(b.RoomCardCount) FROM QPAgent.dbo.AgentRecharge b Where b.UserID in (SELECT UserID FROM QPAccountsDB.dbo.AccountsInfo a Where a.SpreaderID = @dwUserID)
		RETURN @Count
	END


GO
/****** Object:  UserDefinedFunction [dbo].[GetRecommendStatu]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[GetRecommendStatu]
(
	@Statu int
)
	RETURNS NVARCHAR(50)
	AS
	BEGIN
		DECLARE @StatuStr nvarchar(50)

		IF @Statu = 0
		BEGIN
			SET @StatuStr = '未处理'
		END

		ELSE IF @Statu = 1
		BEGIN
			SET @StatuStr = '已处理'
		END
		
		RETURN @StatuStr
	END

GO
/****** Object:  UserDefinedFunction [dbo].[GetWithdrawalStatu]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[GetWithdrawalStatu]
(
	@Statu int		-- 状态值
)
RETURNS NVARCHAR(50)
AS
BEGIN
	DECLARE @Value NVARCHAR(50)
	
	IF @Statu = 0
	begin
		set @Value = '提现申请'
	end

	IF @Statu = 1
	begin
		set @Value = '已打款'
	end

	IF @Statu = 2
	begin
		set @Value = '完成验收'
	end
	RETURN @Value
END

GO
/****** Object:  Table [dbo].[AgentButton]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentButton](
	[BtnID] [int] IDENTITY(1,1) NOT NULL,
	[DataID] [int] NOT NULL,
	[Text] [nvarchar](50) NOT NULL,
	[BoxClass] [nvarchar](50) NOT NULL,
	[BtnClass] [nvarchar](50) NOT NULL,
	[OnClick] [nvarchar](200) NOT NULL,
 CONSTRAINT [PK_AgentButton] PRIMARY KEY CLUSTERED 
(
	[BtnID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentCitys]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentCitys](
	[ID] [int] NOT NULL,
	[Superior] [int] NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
 CONSTRAINT [PK_AgentCitys] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentConfig]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentConfig](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[ManageAccounts] [nvarchar](32) NOT NULL,
	[ManagePassword] [nvarchar](32) NOT NULL,
	[PayRatio] [decimal](18, 2) NOT NULL,
	[THOrQP] [int] NOT NULL,
 CONSTRAINT [PK_AgentConfig] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentControls]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentControls](
	[ControlID] [int] IDENTITY(1,1) NOT NULL,
	[DataID] [int] NOT NULL,
	[Label] [nvarchar](50) NOT NULL,
	[FormName] [nvarchar](50) NOT NULL,
	[Type] [int] NOT NULL,
	[Other] [nvarchar](2000) NULL,
	[Parameter] [nvarchar](50) NULL,
	[Sort] [int] NOT NULL,
 CONSTRAINT [PK_AgentControls] PRIMARY KEY CLUSTERED 
(
	[ControlID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentDistribution]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentDistribution](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[DistributionLevel] [int] NOT NULL,
	[AgentID] [int] NOT NULL,
	[RechargeID] [int] NOT NULL,
	[BeforeMoney] [decimal](18, 2) NOT NULL,
	[DistributionMoney] [decimal](18, 2) NOT NULL,
	[ThenDistributionRatio] [decimal](18, 2) NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentDistribution] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentDistributionLevel]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentDistributionLevel](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[DistributionLevel] [int] NOT NULL,
	[DistributionRatio] [decimal](18, 2) NOT NULL,
	[DistributionUpper] [int] NOT NULL,
 CONSTRAINT [PK_AgentDistributionLevel] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentFeedback]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentFeedback](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[UserID] [int] NOT NULL,
	[Message] [nvarchar](max) NOT NULL,
	[ReleaseDatetime] [datetime] NOT NULL,
	[FeedbackUserID] [int] NOT NULL,
	[FeedbackMessage] [nvarchar](max) NOT NULL,
 CONSTRAINT [PK_AgentFeedback] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentList]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentList](
	[DataID] [int] IDENTITY(1,1) NOT NULL,
	[SidebarID] [int] NOT NULL,
	[ListType] [int] NOT NULL,
	[ListName] [nvarchar](50) NOT NULL,
	[FunName] [nvarchar](max) NOT NULL,
	[Parameter] [nvarchar](1000) NOT NULL,
 CONSTRAINT [PK_AgentList] PRIMARY KEY CLUSTERED 
(
	[DataID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentLogon_log]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentLogon_log](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[LastLogonIP] [nvarchar](15) NOT NULL,
	[LastLogonMachine] [nvarchar](32) NOT NULL,
	[LastLogonDate] [datetime] NOT NULL,
	[LogonType] [int] NOT NULL,
 CONSTRAINT [PK_AgentLogon_log] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentMessage]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentMessage](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[Title] [nvarchar](max) NOT NULL,
	[Message] [nvarchar](max) NOT NULL,
	[ReleaseDatetime] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentMessage] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentPay]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentPay](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[RagentID] [int] NOT NULL,
	[OrderID] [nvarchar](32) NOT NULL,
	[BeforeRoomCard] [decimal](18, 2) NOT NULL,
	[RoomCard] [decimal](18, 2) NOT NULL,
	[Amount] [decimal](18, 2) NOT NULL,
	[Rate] [decimal](18, 2) NOT NULL,
	[Statu] [int] NOT NULL,
	[Rdatetime] [datetime] NOT NULL,
	[ClientIP] [nvarchar](15) NOT NULL,
	[MachineID] [nvarchar](32) NOT NULL,
	[Note] [nvarchar](max) NULL,
 CONSTRAINT [PK_AgentPay] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRebate]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRebate](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[LowerAgentID] [int] NOT NULL,
	[DistributionLevel] [int] NOT NULL,
	[BeforeMoney] [decimal](18, 2) NOT NULL,
	[DistributionMoney] [decimal](18, 2) NOT NULL,
	[ThenDistributionRatio] [decimal](18, 2) NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentRebate_1] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRebateLevel]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRebateLevel](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[DistributionLevel] [int] NOT NULL,
	[DistributionRatio] [decimal](18, 2) NOT NULL,
	[DistributionUpper] [int] NOT NULL,
 CONSTRAINT [PK_AgentRebate] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRecharge]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRecharge](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[UserID] [int] NOT NULL,
	[OrderID] [nvarchar](32) NOT NULL,
	[Amount] [decimal](18, 2) NOT NULL,
	[RoomCardCount] [bigint] NOT NULL,
	[RDatetime] [datetime] NOT NULL,
	[Statu] [int] NOT NULL,
	[Note] [nvarchar](max) NULL,
 CONSTRAINT [PK_AgentRecharge] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRechargeOption]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRechargeOption](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[RoomCardCount] [bigint] NOT NULL,
	[Score] [bigint] NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentRechargeOption] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRecommend]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRecommend](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[RecommendAgentID] [int] NOT NULL,
	[Phone] [nvarchar](16) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[Dispose] [int] NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentRecommend] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentRole]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentRole](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[Role] [nvarchar](max) NOT NULL,
	[IsAdmin] [int] NULL,
	[Identity] [nvarchar](50) NOT NULL,
 CONSTRAINT [PK_AgentRole] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentSidebar]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentSidebar](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[NavIcon] [nvarchar](50) NOT NULL,
	[NavName] [nvarchar](50) NOT NULL,
	[NavUrl] [nvarchar](max) NOT NULL,
	[NavSuperiorID] [int] NULL,
	[NavRole] [nvarchar](50) NOT NULL,
	[Sort] [int] NOT NULL,
	[Nullity] [tinyint] NOT NULL,
 CONSTRAINT [PK_AgentSidebar] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentStatus]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentStatus](
	[StatuID] [int] IDENTITY(1,1) NOT NULL,
	[Value] [nvarchar](50) NOT NULL,
	[Text] [nvarchar](50) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
 CONSTRAINT [PK_AgentStatu] PRIMARY KEY CLUSTERED 
(
	[StatuID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentUser]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentUser](
	[AgentID] [int] IDENTITY(10000,1) NOT NULL,
	[UserID] [int] NOT NULL,
	[Superior] [int] NULL,
	[Accounts] [nvarchar](31) NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[LogonPass] [nvarchar](32) NOT NULL,
	[Header] [nvarchar](max) NULL,
	[Distribution] [decimal](18, 2) NOT NULL,
	[Money] [decimal](18, 2) NOT NULL,
	[RoomCard] [decimal](18, 2) NOT NULL,
	[Compellation] [nvarchar](16) NOT NULL,
	[PassPortID] [nvarchar](18) NOT NULL,
	[Phone] [nvarchar](16) NOT NULL,
	[CityID] [int] NOT NULL,
	[CardIDImg] [nvarchar](max) NULL,
	[SelfImg] [nvarchar](max) NULL,
	[Nullity] [int] NOT NULL,
	[RegisterDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentUser] PRIMARY KEY CLUSTERED 
(
	[AgentID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentUserGame]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentUserGame](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[UserIDs] [nvarchar](200) NOT NULL,
	[AgentIDs] [nvarchar](200) NOT NULL,
	[BottomNote] [decimal](18, 2) NOT NULL,
	[PumpingRate] [decimal](18, 2) NOT NULL,
	[Proportions] [nvarchar](200) NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentUserGame] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[AgentWithdrawal]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AgentWithdrawal](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AgentID] [int] NOT NULL,
	[Withdrawal] [decimal](18, 2) NOT NULL,
	[Statu] [int] NOT NULL,
	[Note] [nvarchar](max) NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_AgentWithdrawal] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[BlackMessages]    Script Date: 2017/8/31 16:16:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[BlackMessages](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[DataName] [nvarchar](50) NULL,
	[Value] [nvarchar](200) NULL,
	[Num] [int] NOT NULL,
	[ApplyDate] [datetime] NOT NULL,
 CONSTRAINT [PK_dbo.BlackMessages] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
ALTER TABLE [dbo].[AgentCitys] ADD  CONSTRAINT [DF_AgentCitys_Superior]  DEFAULT ((0)) FOR [Superior]
GO
ALTER TABLE [dbo].[AgentConfig] ADD  CONSTRAINT [DF_AgentConfig_ManagePassword]  DEFAULT ('') FOR [ManagePassword]
GO
ALTER TABLE [dbo].[AgentConfig] ADD  CONSTRAINT [DF_AgentConfig_PayRatio]  DEFAULT ((1)) FOR [PayRatio]
GO
ALTER TABLE [dbo].[AgentConfig] ADD  CONSTRAINT [DF_AgentConfig_THOrQP]  DEFAULT ((0)) FOR [THOrQP]
GO
ALTER TABLE [dbo].[AgentControls] ADD  CONSTRAINT [DF_AgentControls_Sort]  DEFAULT ((0)) FOR [Sort]
GO
ALTER TABLE [dbo].[AgentDistribution] ADD  CONSTRAINT [DF_AgentDistribution_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentDistributionLevel] ADD  CONSTRAINT [DF_AgentDistributionLevel_DistributionUpper]  DEFAULT ((0)) FOR [DistributionUpper]
GO
ALTER TABLE [dbo].[AgentFeedback] ADD  CONSTRAINT [DF_AgentFeedback_Message]  DEFAULT (N'') FOR [Message]
GO
ALTER TABLE [dbo].[AgentFeedback] ADD  CONSTRAINT [DF_AgentFeedback_ReleaseDatetime]  DEFAULT (getdate()) FOR [ReleaseDatetime]
GO
ALTER TABLE [dbo].[AgentFeedback] ADD  CONSTRAINT [DF_AgentFeedback_FeedbackUserID]  DEFAULT ((0)) FOR [FeedbackUserID]
GO
ALTER TABLE [dbo].[AgentFeedback] ADD  CONSTRAINT [DF_AgentFeedback_FeedbackMessage]  DEFAULT (N'') FOR [FeedbackMessage]
GO
ALTER TABLE [dbo].[AgentList] ADD  CONSTRAINT [DF_AgentList_ListType]  DEFAULT ((1)) FOR [ListType]
GO
ALTER TABLE [dbo].[AgentList] ADD  CONSTRAINT [DF_AgentList_ListName]  DEFAULT (N'N('')') FOR [ListName]
GO
ALTER TABLE [dbo].[AgentLogon_log] ADD  CONSTRAINT [DF_AgentLogon_log_LastLogonDate]  DEFAULT (getdate()) FOR [LastLogonDate]
GO
ALTER TABLE [dbo].[AgentMessage] ADD  CONSTRAINT [DF_AgentMessage_ReleaseDatetime]  DEFAULT (getdate()) FOR [ReleaseDatetime]
GO
ALTER TABLE [dbo].[AgentPay] ADD  CONSTRAINT [DF_AgentPay_Rdatetime]  DEFAULT (getdate()) FOR [Rdatetime]
GO
ALTER TABLE [dbo].[AgentPay] ADD  CONSTRAINT [DF_AgentPay_ClientIP]  DEFAULT (N'') FOR [ClientIP]
GO
ALTER TABLE [dbo].[AgentPay] ADD  CONSTRAINT [DF_AgentPay_MachineID]  DEFAULT (N'') FOR [MachineID]
GO
ALTER TABLE [dbo].[AgentRebate] ADD  CONSTRAINT [DF_AgentRebate_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentRechargeOption] ADD  CONSTRAINT [DF_AgentRechargeOption_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentRecommend] ADD  CONSTRAINT [DF_AgentRecommend_Dispose]  DEFAULT ((0)) FOR [Dispose]
GO
ALTER TABLE [dbo].[AgentRecommend] ADD  CONSTRAINT [DF_AgentRecommend_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentRole] ADD  CONSTRAINT [DF_AgentRole_Role]  DEFAULT ('PT') FOR [Role]
GO
ALTER TABLE [dbo].[AgentRole] ADD  CONSTRAINT [DF_AgentRole_IsAdmin]  DEFAULT ((0)) FOR [IsAdmin]
GO
ALTER TABLE [dbo].[AgentSidebar] ADD  CONSTRAINT [DF_AgentSidebar_NavUrl]  DEFAULT ('') FOR [NavUrl]
GO
ALTER TABLE [dbo].[AgentSidebar] ADD  CONSTRAINT [DF_AgentSidebar_NavRole]  DEFAULT ('PT') FOR [NavRole]
GO
ALTER TABLE [dbo].[AgentSidebar] ADD  CONSTRAINT [DF_AgentSidebar_Sort]  DEFAULT ((0)) FOR [Sort]
GO
ALTER TABLE [dbo].[AgentSidebar] ADD  CONSTRAINT [DF_AgentSidebar_Nullity]  DEFAULT ((0)) FOR [Nullity]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_Name]  DEFAULT ('') FOR [Name]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_Money]  DEFAULT ((0)) FOR [Money]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_RoomCard]  DEFAULT ((0)) FOR [RoomCard]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_Compellation]  DEFAULT (N'N('')') FOR [Compellation]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_PassPortID]  DEFAULT (N'N('')') FOR [PassPortID]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_Phone]  DEFAULT (N'N('')') FOR [Phone]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_Nullity]  DEFAULT ((0)) FOR [Nullity]
GO
ALTER TABLE [dbo].[AgentUser] ADD  CONSTRAINT [DF_AgentUser_RegisterDate]  DEFAULT (getdate()) FOR [RegisterDate]
GO
ALTER TABLE [dbo].[AgentUserGame] ADD  CONSTRAINT [DF_AgentUserGame_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentWithdrawal] ADD  CONSTRAINT [DF_AgentWithdrawal_Note]  DEFAULT ('') FOR [Note]
GO
ALTER TABLE [dbo].[AgentWithdrawal] ADD  CONSTRAINT [DF_AgentWithdrawal_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[BlackMessages] ADD  CONSTRAINT [DF_BlackMessages_ApplyDate]  DEFAULT (getdate()) FOR [ApplyDate]
GO
ALTER TABLE [dbo].[AgentControls]  WITH CHECK ADD  CONSTRAINT [FK_AgentControls_AgentList] FOREIGN KEY([DataID])
REFERENCES [dbo].[AgentList] ([DataID])
GO
ALTER TABLE [dbo].[AgentControls] CHECK CONSTRAINT [FK_AgentControls_AgentList]
GO
ALTER TABLE [dbo].[AgentList]  WITH CHECK ADD  CONSTRAINT [FK_Sidebar_1_Table_1] FOREIGN KEY([DataID])
REFERENCES [dbo].[AgentList] ([DataID])
GO
ALTER TABLE [dbo].[AgentList] CHECK CONSTRAINT [FK_Sidebar_1_Table_1]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'1 : TextBox  2 : ' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentControls', @level2type=N'COLUMN',@level2name=N'Type'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'代理商ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'AgentID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'充值ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'RechargeID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'分销前代理商金额' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'BeforeMoney'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'分销金额' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'DistributionMoney'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'分销时的分销比例' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'ThenDistributionRatio'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'分销时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentDistribution', @level2type=N'COLUMN',@level2name=N'ApplyDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'登录类型 0：首次登录   1：正常登录' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentLogon_log', @level2type=N'COLUMN',@level2name=N'LogonType'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRecharge', @level2type=N'COLUMN',@level2name=N'UserID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'充值金额' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRecharge', @level2type=N'COLUMN',@level2name=N'Amount'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'充值时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRecharge', @level2type=N'COLUMN',@level2name=N'RDatetime'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRole', @level2type=N'COLUMN',@level2name=N'AgentID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'权限  （默认是PT，如有多个权限，请使用 , 分隔）' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRole', @level2type=N'COLUMN',@level2name=N'Role'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否是管理员' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentRole', @level2type=N'COLUMN',@level2name=N'IsAdmin'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'自增长代理标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentUser', @level2type=N'COLUMN',@level2name=N'AgentID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'上级' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentUser', @level2type=N'COLUMN',@level2name=N'Superior'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'分销比例' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentUser', @level2type=N'COLUMN',@level2name=N'Distribution'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'代理商ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentWithdrawal', @level2type=N'COLUMN',@level2name=N'AgentID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'提现金额' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentWithdrawal', @level2type=N'COLUMN',@level2name=N'Withdrawal'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'状态 （0：提现申请  1：已打款  2：完成验收）' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentWithdrawal', @level2type=N'COLUMN',@level2name=N'Statu'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'备注' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentWithdrawal', @level2type=N'COLUMN',@level2name=N'Note'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'AgentWithdrawal', @level2type=N'COLUMN',@level2name=N'ApplyDate'
GO
USE [master]
GO
ALTER DATABASE [QPAgent] SET  READ_WRITE 
GO
