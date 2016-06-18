local ffi=require 'ffi'

local C = ffi.load(package.searchpath('libpseudocount', package.cpath))

ffi.cdef([[
void* init();
double probability(void*, void*);
void finish(void*);
]]);

local x = torch.ByteTensor(10,10)


local tree = C.init();


local p
for i=1,1000 do
    p = C.probability(tree, x:data());
end

print(p);

C.finish(tree);