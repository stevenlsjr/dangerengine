
local fn = require('functional')

--- utilities

function printf(fmt, ... )
    return io.write(fmt:format(...))
end -- printf


--- program logic

local tileMesh


function Vertex( position, uv, color )
    return {position[1], position[2],
            uv[1], uv[2],
            color[1], color[2], color[3], color[4]}
end

function makeTileMesh( tileTable )
    -- body
    local tileMap = {
        {0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 1, 1, 0, 1, 0},
        {0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0}
    }

    local unitLen =  1.0

    local vertices = {}
    local uvs = {{0.0, 0.0},
                 {0.0, 1.0},
                 {1.0, 1.0},
                 {1.0, 0.0}}
     local 

    for j, row in ipairs(tileMap) do
        for i, col in ipairs(row) do
            printf('%i, %i: {%s}\n', i, j, table.concat(tileTable[col], ','))

        end -- for
        printf('\n')
    end -- for

    return nil
end


---love game callbacks


function love.load( )
    local tileTable = {
        [0] = {255, 255, 255, 0},
        [1] = {255, 255, 255, 255},
        [2] = {255, 0,   0,   255},
        [3] = {0,   255, 0,   255},
        [4] = {0,   0,   255, 255}
    }
    print(tileTable)
    tileMesh = makeTileMesh(tileTable)
end

function love.draw()
    love.graphics.print('hello!', 400, 300)
end


