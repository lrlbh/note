drop database if exists admin_gl;
set names utf8;
create database if not exists admin_gl default charset utf8;
use admin_gl;

create table jsy(
	jsy_id bigint unsigned primary key,
	jsy_name char(50) not null,
	sex char(1) not null  default '男' check(sex in('男','女')),
	birthdate date not null,
	instructor_id bigint unsigned,
	foreign key(instructor_id) references jsy(jsy_id)
);

INSERT INTO jsy (jsy_id,jsy_name,sex,birthdate,instructor_id)VALUES
(1,'文侯','男',"1994-11-10",NULL),
(2,'武侯','男',"1994-11-10",NULL),
(3,'枫','女',"1994-11-10",NULL),
(4,'沈西平',default,"1994-11-10",2),
(5,'陆经鱼','和',"1994-11-10",2),
(6,'毕炜','男',"1994-11-10",1),
(7,'邓沧澜','男',"1994-11-10",1),
(8,'邵风观','男',"1994-11-10",1),
(9,'茵郡主','女',"1994-11-10",null),
(10,'南武公子','男',"1994-11-10",NULL),
(11,'丁亨利','男',"1994-11-10",5),
(12,'楚休红','男',"1994-11-10",2),
(13,'武昭','男',"1994-11-10",NULL),
(14,'小王子','男',"1994-11-10",13),
(15,'苏纹月','女',"1994-11-10",NULL),
(16,'路恭行','男',"1994-11-10",null),
(17,'钱文义','男',"1994-11-10",12),
(18,'陈忠','男',"1994-11-10",12),
(19,'柳风舞','男',"1994-11-10",12);


create table xm(
	xm_id bigint unsigned primary key,
	xm_name char(50) not null,
	startdate datetime not null,
	enddate datetime
);


INSERT INTO xm(xm_id,xm_name,startdate,enddate)VALUES
(1,'高？城守卫',"1994-11-10","1994-11-10"),
(2,'保卫帝都',"1994-11-10","1994-11-10"),
(3,'投降共和',"1994-11-10","1994-11-10");

create table zjb(
	xm_id bigint unsigned not null,
	jsy_id bigint unsigned not null,
	foreign key (xm_id) references xm(xm_id),
	foreign key (jsy_id) references jsy(jsy_id)
);

INSERT INTO zjb(xm_id,jsy_id)VALUES
(1,1),
(1,2),
(1,3),
(1,4),
(1,5),
(1,12),
(1,15),
(1,16),
(1,17),
(2,1),
(2,3),
(2,6),
(2,7),
(2,8),
(2,9),
(2,12),
(2,13),
(2,14),
(2,16),
(2,17),
(2,18),
(3,3),
(3,6),
(3,7),
(3,8),
(3,10),
(3,11),
(3,12),
(3,14),
(3,17),
(3,18);

set names gbk;
-- 1、	指定一名技术人员，可查询直接指导他（她）的技术人员的信息；
SELECT b.*
FROM jsy as a,jsy as b
WHERE a.instructor_id=b.jsy_id
-- AND a.jsy_id=18，
and a.jsy_name='陈忠';

-- 2、	指定一名技术人员，可查询他（她）直接指导的其它技术人员的信息；
SELECT a.*
FROM jsy as a,jsy as b
WHERE a.instructor_id=b.jsy_id;
-- AND b.jsy_name='楚休红';

-- 3、	指定一名技术人员，可查询他（她）所参与的所有项目的信息；
SELECT xm_name '(姓名)'
FROM jsy,zjb,xm
WHERE jsy.jsy_id=1
and xm.xm_id=zjb.xm_id
and jsy.jsy_id=zjb.jsy_id;

-- 4、	指定一个项目，可查询参与此项目的所有技术人员信息。
SELECT jsy_name as 姓名
FROM jsy,zjb,xm
WHERE xm.xm_id =1
and xm.xm_id=zjb.xm_id
and jsy.jsy_id=zjb.jsy_id;