local ffi=require 'ffi'
local x = torch.FloatTensor(2)

local C = ffi.load(package.searchpath('libpseudocount', package.cpath))

x[1]=100
x[2]=200


