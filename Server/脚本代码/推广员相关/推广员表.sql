USE 
	[QPTreasureDB]
GO
CREATE TABLE Popularize
(
	popularize_id int identity(1,1) primary key,  
	uid bigint,
	name varchar(20),
	code varchar(40),
	start_time datetime,
	end_time datetime
)
