local ffi=require 'ffi'

local C = ffi.load(package.searchpath('libpseudocount', package.cpath))

ffi.cdef([[
void* init();
double probability(void*, void*);
void finish(void*);
]]);

local x = torch.ByteTensor(10,10)
x[1]=100
x[2]=200


local tree = C.init();

for i=1,10000 do
local p = C.probability(tree, x:data());
print(p);
end

C.finish(tree);