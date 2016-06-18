local classic = require 'classic'
local ffi=require 'ffi'

ffi.cdef([[
void* init(uint16_t);
double probability(void*, void*);
void finish(void*);
]]);

local C = ffi.load(package.searchpath('libpseudocount', package.cpath))

local PseudoCount = classic.class('PseudoCount')

function PseudoCount:_init(dim)
    self.tree = C.init(dim)
    classic.strict(self)
end

function PseudoCount:probability(screen)
    return tonumber(C.probability(self.tree, screen:data()))
end

function PseudoCount:finish()
    C.finish(self.tree)
end

return PseudoCount;