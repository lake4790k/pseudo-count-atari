local ffi=require 'ffi'
ffi.cdef[[ void func(float*) ]]
local x = torch.FloatTensor(2)

local C = ffi.load('libpseudocount.dylib')
--local C = ffi.load('libpseudocount.so')

x[1]=100
x[2]=200

C.func(x:data())
