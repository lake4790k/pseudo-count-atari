local PseudoCount = require 'pseudocount';

local Test = torch.TestSuite()
local tester = torch.Tester()

torch.manualSeed(1)

function Test:test()
    local dim = 42
    local count = PseudoCount(dim)
    local screen = torch.ByteTensor(dim,dim):fill(0)

    local p
    for i=1,100 do
        p = count:pseudoCount(screen)
        print(p);
    end
    tester:assertge(p, 15)
    count:finish()
end

tester:add(Test)
tester:run()
