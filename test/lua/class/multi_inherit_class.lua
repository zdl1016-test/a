
local Account={
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

local Named = {
    name = "nobody"
}

function Named:getname()
    return self.name
end

function Named:setname(name)
    self.name = name
end

-- 在每一个基类中寻找是否有指定的k
-- plist 是个数组, 其中每一个元素都是个基类table
local function search (k, plist)
    for i=1, #plist do
        local v = plist[i][k]
        if v then return v end
    end
end

-- 创建一个子类的类原型
-- 可以具有多个参数, 同时派生自多个基类
local function createClass(...)
    local c = {}
    local parent = {...}

    -- 设置新的子类原型的元表 当找不到k时, 从各个基类中搜
    setmetatable(c, {__index = 
        function(table, k) 
            return search(k, parent) 
        end} 
    )

    -- 未来子类的实例的元表就是子类原型了
    c.__index = c;

    function c:new(o)
        local o = o or {}
        setmetatable(o, c)
        return o
    end

    return c
end

local NameedAccount = createClass(Account, Named)
local o = NameedAccount:new()
o:setname("nemo")
print(o:getname())
o:deposit(100)
print(o.balance)

