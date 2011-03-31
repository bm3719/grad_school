-- Bruce Miller, Farzaneh Asgari, Michael Schultz
-- createtable.sql

SPOOL dbms4.out;
SET ECHO ON;
SET TIME ON;
SET LINESIZE 80;

-- drop the tables and types being used
DROP TABLE investor     CASCADE CONSTRAINTS;    
DROP TABLE account      CASCADE CONSTRAINTS;    
DROP TABLE stocks       CASCADE CONSTRAINTS;    
DROP TABLE investments  CASCADE CONSTRAINTS;
DROP TABLE owns         CASCADE CONSTRAINTS;    
DROP TABLE listed       CASCADE CONSTRAINTS;    
DROP TABLE debits       CASCADE CONSTRAINTS;    
DROP TABLE credits      CASCADE CONSTRAINTS;    

DROP TYPE name1_type;
DROP TYPE addr_type;
DROP TYPE phone_type;
DROP TYPE name2_type;
DROP TYPE money_type;
DROP TYPE date_type;
DROP TYPE num_type;

-- create the object types
CREATE TYPE name1_type AS OBJECT(
        fname   VARCHAR2(10),
        minit   CHAR,
        lname   VARCHAR2(10));
/
CREATE TYPE addr_type AS OBJECT(
        st_num  VARCHAR2(6),
        street  VARCHAR2(12));
/
CREATE TYPE phone_type AS OBJECT(
        area_code       CHAR(3),
        phone_num       CHAR(7));
/
CREATE TYPE name2_type AS OBJECT(
        name    VARCHAR(16));
/
CREATE TYPE money_type AS OBJECT(
        value   DECIMAL(9,2));
/
CREATE TYPE date_type AS OBJECT(
        day     INT,
        month   INT,
        year    INT);
/
CREATE TYPE num_type AS OBJECT(
        num     CHAR(6));
/        

-- create tables
CREATE TABLE investor (
        name    name1_type, 
        ssn     CHAR(9)         NOT NULL, 
        address addr_type, 
        phone   phone_type, 
        bdate   date_type,
        PRIMARY KEY (ssn) );

CREATE TABLE account(
        balance    money_type,
        acctno     CHAR(6)      NOT NULL,
        acc_type   num_type,
        invssn     CHAR(9)      NOT NULL,
        PRIMARY KEY(acctno),
        FOREIGN KEY (invssn) REFERENCES INVESTOR(ssn));

CREATE TABLE investments(
        code    CHAR(6)         NOT NULL,
        value   money_type,
        company name2_type,
        type    name2_type,
        invssn  CHAR(9)         NOT NULL,
        acctno  CHAR(6)         NOT NULL,
        PRIMARY KEY(code),
        FOREIGN KEY(invssn) REFERENCES INVESTOR(ssn),
        FOREIGN KEY(acctno) REFERENCES ACCOUNT(acctno));

CREATE TABLE stocks(
        symbol  VARCHAR(4)      NOT NULL,
        name    name2_type,
        value   money_type,
        PRIMARY KEY(symbol));

CREATE TABLE owns (
        invssn  CHAR(9) NOT NULL,
        symbol  VARCHAR(4)      NOT NULL,
        amount  num_type,
        PRIMARY KEY (invssn, symbol),
        FOREIGN KEY (symbol) REFERENCES STOCKS(symbol),
        FOREIGN KEY (invssn) REFERENCES INVESTOR(ssn));

CREATE TABLE listed (
        symbol  VARCHAR(4)      NOT NULL,
        acctno  CHAR(6)         NOT NULL,
        amount  num_type,
        PRIMARY KEY (symbol, acctno),
        FOREIGN KEY (symbol) REFERENCES STOCKS(symbol),
        FOREIGN KEY (acctno) REFERENCES ACCOUNT(acctno));

CREATE TABLE debits(
        billacct        CHAR(6)         NOT NULL,
        on_date         date_type,
        billbank        name2_type,
        amt             money_type,
        recipient       name2_type,
        billacctno      CHAR(6)         NOT NULL,
        PRIMARY KEY (billacct),
        FOREIGN KEY (billacctno) REFERENCES ACCOUNT(acctno));
        
CREATE TABLE credits(
        sendacctno      CHAR(6)         NOT NULL,
        sender          name2_type,
        amt             money_type,
        sendbank        name2_type,
        on_date         date_type,
        addacctno       CHAR(6)         NOT NULL,
        PRIMARY KEY (sendacctno),
        FOREIGN KEY (addacctno) REFERENCES ACCOUNT(acctno));

-- describe tables defined
DESCRIBE investor;
DESCRIBE account;
DESCRIBE stocks;
DESCRIBE owns;
DESCRIBE listed;
DESCRIBE debits;
DESCRIBE credits;
        
-- sample data 

INSERT INTO investor VALUES (name1_type('Bruce', 'C', 'Miller'), '111223333', 
        addr_type('515', 'East Orange'), phone_type('717', '5325605'), 
        date_type(15, 7, 1978));
INSERT INTO investor VALUES (name1_type('Farzaneh', 'X', 'Asgari'), '222334444',
        addr_type('32', 'South Earl'), phone_type('717', '5324444'), 
        date_type(2, 7, 1979));
INSERT INTO investor VALUES (name1_type('Michael', 'L', 'Schultz'), '333445555',
        addr_type('11', 'North Main'), phone_type('717', '5323232'), 
        date_type(1, 4, 1989));
INSERT INTO investor VALUES (name1_type('Steve', 'Q', 'Smith'), '444556666', 
        addr_type('44', 'South Queen'), phone_type('717', '5324523'), 
        date_type(11, 2, 1990));
INSERT INTO investor VALUES (name1_type('Dingo', 'Z', 'Farm'), '555667777', 
        addr_type('498', 'North Prince'), phone_type('717', '5327777'), 
        date_type(12, 3, 1943));

INSERT INTO account VALUES(money_type(320000.00), '000001', 
        num_type('100000'), '111223333');
INSERT INTO account VALUES(money_type(520000.01), '000002', 
        num_type('100000'), '222334444');
INSERT INTO account VALUES(money_type(100000.00), '000003', 
        num_type('100000'), '333445555');
INSERT INTO account VALUES(money_type(525555.50), '000004', 
        num_type('100000'), '444556666');
INSERT INTO account VALUES(money_type(1200.00), '000005', 
        num_type('100000'), '555667777');

INSERT INTO stocks VALUES('IBM',name2_type('IBM'),money_type(32.25));
INSERT INTO stocks VALUES('CSC',name2_type('Cisco'),money_type(30.25));
INSERT INTO stocks VALUES('NOR',name2_type('Nortel'),money_type(22.25));
INSERT INTO stocks VALUES('HIN',name2_type('Holiday'),money_type(12.25));
INSERT INTO stocks VALUES('WAM',name2_type('WalMart'),money_type(52.25));

INSERT INTO investments VALUES('100001', money_type(25000.00), 
        name2_type('Fidelity'), name2_type('Mutual_fund'),
        '111223333', '000001');
INSERT INTO investments VALUES('100002', money_type(35000.00), 
        name2_type('Prudential'), name2_type('Mutual_fund'), 
        '222334444', '000002');
INSERT INTO investments VALUES('100003', money_type(55000.00), 
        name2_type('Oppenhiemer'), name2_type('Roth_IRA'),
        '333445555','000003');
INSERT INTO investments VALUES('100004', money_type(21000.00), 
        name2_type('Opeen'), name2_type('IRA'),
        '111223333','000001');
INSERT INTO investments VALUES('100005', money_type(15000.00), 
        name2_type('Fidelity'), name2_type('Mutual_fund'),
        '222334444','000002');

INSERT INTO debits VALUES('200001', date_type(12,10,2002), 
        name2_type('MTT'), money_type(12000.00), 
        name2_type('coffins.com'), '000001');
INSERT INTO debits VALUES('200002', date_type(12,07,2003), 
        name2_type('FM'), money_type(22000.00), 
        name2_type('crayons.com'), '000005');
INSERT INTO debits VALUES('200003', date_type(10,10,2002), 
        name2_type('Federal Union'), money_type(10000.00), 
        name2_type('dde.com'), '000004');
INSERT INTO debits VALUES('200004', date_type(12,10,2002), 
        name2_type('Allfirst'), money_type(13000.00), 
        name2_type('coffins.com'), '000003');
INSERT INTO debits VALUES('200005', date_type(07,11,2000), 
        name2_type('MTT'), money_type(15000.00), 
        name2_type('ffer.com'), '000002');

INSERT INTO credits VALUES('400001', name2_type('Sperm Bank'), 
        money_type(34.30), name2_type('Members first'), 
        date_type(12, 9, 1999), '000001');
INSERT INTO credits VALUES('400002', name2_type('Fhqwgads'), 
        money_type(4534.20), name2_type('Federal Credit'), 
        date_type(1, 29, 1997), '000002');
INSERT INTO credits VALUES('400003', name2_type('Lawn Doctors'), 
        money_type(46.00), name2_type('Allfirst'), 
        date_type(4, 19, 2000), '000003');
INSERT INTO credits VALUES('400004', name2_type('Thift Store'), 
        money_type(4.25), name2_type('FM'), 
        date_type(3, 7, 1995), '000004');
INSERT INTO credits VALUES('400005', name2_type('Jump School'), 
        money_type(7.01), name2_type('Members first'), 
        date_type(1, 12, 2003), '000005');

INSERT INTO owns VALUES('111223333', 'IBM', num_type('000002'));
INSERT INTO owns VALUES('222334444', 'CSC', num_type('000001'));
INSERT INTO owns VALUES('333445555', 'NOR', num_type('000002'));
INSERT INTO owns VALUES('444556666', 'HIN', num_type('000001'));
INSERT INTO owns VALUES('111223333', 'WAM', num_type('000001'));

INSERT INTO listed VALUES('IBM', '000001', num_type('000002'));
INSERT INTO listed VALUES('CSC', '000002', num_type('000001'));
INSERT INTO listed VALUES('NOR', '000003', num_type('000002'));
INSERT INTO listed VALUES('HIN', '000004', num_type('000001'));
INSERT INTO listed VALUES('WAM', '000001', num_type('000001'));


-- sample queries 

-- 1.   List the name and account number of the investor 
--      that has balance more than  $100,000.

SELECT  name, acctno
FROM    investor, account a
WHERE   ssn = invssn AND a.balance.value> 100000.00;


-- 2.   retrieve the name of all investor and account numbers that 
--      are invested in the mutual fund.

SELECT   name, acctno
FROM     investor, investments v
WHERE    ssn = invssn AND v.type.name ='Mutual_fund';

-- 3.   find the sum of the loans by MTT bank.

SELECT   sum(d.amt.value)
FROM     debits d
WHERE   d.billbank.name='MTT';

-- 4.   list the names and account numbers of investors who have 
--      have no stocks

SELECT  name, acctno
FROM    investor, account
WHERE   ssn=invssn AND NOT EXISTS
(SELECT     *
  FROM      owns
   WHERE   ssn = invssn) ;

-- 5.   list the account type that recieved
--      credit from Members first.

SELECT  acc_type
FROM    account, credits c
WHERE   acctno = addacctno 
AND c.sendbank.name ='Members first';

-- 6. display all names of investors born after 1978.

SELECT name  
FROM investor i
WHERE i.bdate.year > 1978;

-- 7. display the all of the total number of stocks 
--    owned

SELECT sum(o.amount.num)
FROM investor i, owns o
WHERE o.invssn = i.ssn;

-- 8. display a list of banks sending credits and the 
--    account type money is sent to

SELECT sendbank, acc_type
FROM credits c, account a
WHERE a.acctno=c.addacctno;

-- 9. display a list of investor names that do not 
--    have debits on their accounts

SELECT i.name
FROM investor i, account a
WHERE i.ssn=a.invssn 
      AND NOT EXISTS 
        ( SELECT * 
          FROM account a, debits d
          WHERE a.acctno = d.billacct );

-- 10. display investment codes along with the 
--     cooresponding account numbers

SELECT code, i.acctno
FROM investments i, account a
WHERE i.acctno = a.acctno;

-- 11. Make a list of all socks for investor, whose 
--     last name is smith., or first name is Dingo

(SELECT symbol
FROM  investor i, owns
WHERE ssn=invssn AND i.name.lname ='Smith')
UNION
(SELECT symbol
FROM  investor i, owns
WHERE ssn=invssn AND i.name.lname ='Dingo');

-- 12. Select those investors whose stock values are 
--     below $10, etc.

SELECT DISTINCT i.ssn 
FROM investor i, owns o, stocks s
WHERE i.ssn = o.invssn AND o.symbol = s.symbol AND s.value.value < 20;

-- 13. Select the accounts that own Nortel stock.

SELECT i.name, i.ssn, i.address, o.symbol, o.amount 
FROM owns o, investor i
WHERE o.symbol = 'NOR';

-- 14. Select only those investors which live in the 717 area code.

SELECT *
FROM investor i
WHERE i.phone.area_code = 717;

-- 15. Select all the stocks which an investor has.

SELECT i.ssn, o.symbol, s.name, s.value
FROM stocks s, owns o, investor i
WHERE i.ssn = o.invssn AND o.symbol = s.symbol;

-- example employee table of famous british authors for the recursive query

DROP TABLE employee;

CREATE TABLE employee (
        name    name1_type,
        ssn     CHAR(9)         NOT NULL,
        mgrssn  CHAR(9),
        PRIMARY KEY (ssn) );

DESCRIBE employee;

INSERT INTO employee VALUES (name1_type('Joseph', 'H', 'Conrad'), '123456789', '198419841');
INSERT INTO employee VALUES (name1_type('G', 'K', 'Chesterson'), '987654321', '123456789');
INSERT INTO employee VALUES (name1_type('Bernard', 'P', 'Shaw'), '198419841', NULL);
INSERT INTO employee VALUES (name1_type('Stephen', 'X', 'Donalson'), '060606060', '198419841');


