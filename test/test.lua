local PseudoCount = require 'pseudocount';

local Test = torch.TestSuite()
local tester = torch.Tester()


function Test:test()
    local dim = 5
    local count = PseudoCount(dim)
    local screen = torch.ByteTensor(dim,dim):fill(0)

    local p
    for i=1,100 do
        p = count:probability(screen)
    end
--    print(p);
    tester:assertge(p, 0.85)

    count:finish()
end

tester:add(Test)
tester:run()
