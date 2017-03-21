d = require 'print_tb'

-- 用table描述一个类原型
Account={
    balance = 0,
    withdraw = function (self, v)
        if self.balance < v then error "insuficient fouds" end
        self.balance = self.balance -v
    end
}

-- 增加一个方法
function Account.deposit(self, v)
    self.balance = self.balance + v
end

-- 再增加一个方法
function Account:new(o)
    o = o or {}
    setmetatable(o, self);
    -- 让Account作为Account实例的元表
    self.__index = self -- 可以放在外面???
    return o
end

-- 实例化一个类
o = Account:new({})
o.balance = 4 --此时, o具有了自己的balance, 不再受类原型的balance变化的影响
print(o.balance)
o:deposit(100)
print(o.balance)

--如果打算将table Account作为基类, 那么最好不要随便改变他得内部值,会影响到后续新创建的子类
--以及会影响到已经创建的,但没有对应的成员变量的子类!!! 
--Account:deposit(3)

--再实例化一个类
o2 = Account:new({})
o2:deposit(10)
print(o2.balance)


