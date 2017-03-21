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

	-- 这里只需要__index即可, 不需要__newindex, 因为__newindex是用来更新的, 当更新的时候, 就让子类生成自己的成员实例, 不要共用基类的. 
    self.__index = self 
    return o
end

-- 定义一个具有limit变量的子类的类原型
SpecialAccount = Account:new{limit = 100}
-- 重写子类的withdraw方法,允许透支limit
function SpecialAccount:withdraw(v)
    if (self.balance + self.limit) >= v then
        self.balance = self.balance - v
    else
        error "insuficient founds in SpecailAccount"
    end
end

--再实例化一个类
o2 = SpecialAccount:new({})
--o2 = Account:new({})
--o2:deposit(10)
print(o2.balance)
o2:withdraw(101)
print(o2.balance)


