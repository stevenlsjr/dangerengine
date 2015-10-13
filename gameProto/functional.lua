
local docstrings = setmetatable({}, {__mode='kv'})

local mod = {}

function mod.document( str )
    return function ( obj )
        if obj then
          docstrings[obj] = str
        end
        return obj
    end
end

function mod.help( obj )
    print (docstrings[obj])
end

mod.document[[Print documentation of given object]](mod.help)
mod.document[[add a document string for object]](mod.document)


function mod.map(fn, arr)
    local mapped = {}
    for i, v in ipairs(arr) do
        mapped[i] = fn(v, i)
    end
    return mapped
end


function mod.filter( fn, arr )
    local filtered = {}
    for i, v in ipairs(array) do
        if (fn(v, i)) then
            filtered.insert(v)
        end
    end
    return filtered
end

function mod.reduce(fn, arr, initial)
    local reduced = initial

    for i, v in ipairs(arr) do
        reduced = fn(reduced, v, i)
    end

    return reduced
end

function mod.sum(arr)
    return mod.reduce(function ( a, b ) return a + b end, arr, 0)
end


return mod



