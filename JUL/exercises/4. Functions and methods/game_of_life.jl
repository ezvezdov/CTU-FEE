function neighbours(world,row,col)
    row_max,col_max = size(world)
    
    col_left = mod1(col-1,col_max)
    col_right = mod1(col+1,col_max)
    row_up = mod1(row-1,row_max)
    row_down = mod1(row+1,row_max)
    
    return (world[row_up,col_left] + world[row_up,col] + world[row_up,col_right]
            + world[row,col_left] + world[row,col_right] + world[row_down,col_left]
            + world[row_down,col] + world[row_down,col_right])
end

function neighbours(world)
    row_max,col_max = size(world)

    neighbours_matrix = zeros(row_max,col_max)

    for i in 1:row_max
        for j in 1:col_max
            neighbours_matrix[i,j] = neighbours(world,i,j)
        end
    end
    return neighbours_matrix
end

function willsurvive(cell_state, neighbours_amount)
    # Conditions
    # Any live cell with exactly two or three live neighbours survives.
    # Any dead cell with exactly three live neighbours becomes a live cell.
    # All other live cells die in the next generation. All other dead cells stay dead.

    cell_state && (neighbours_amount == 2 || neighbours_amount == 3) && return true
    !cell_state && neighbours_amount == 3 && return true
    return false
end

function evolve!(world)
    row_max,col_max = size(world)

    neighbours_matrix = neighbours(world)
    for i in 1:row_max
        for j in 1:col_max
            world[i,j] = willsurvive(world[i,j],neighbours_matrix[i,j])
        end
    end
    return
end

# gameoflife_1.gif
# world = zeros(Bool, 30, 30)
# row, col = 15, 15
# world[row, col] = 1
# world[row, col + 1] = 1
# world[row - 1, col + 6] = 1
# world[row + 1, col + 1] = 1
# world[row + 1, col + 5] = 1
# world[row + 1, col + 6] = 1
# world[row + 1, col + 7] = 1

# gameoflife_2.gif
world = zeros(Bool, 17, 17)
line = zeros(17)
line[5:7] .= 1
line[11:13] .= 1

for ind in [3,8,10,15]
    world[ind, :] .= line
    world[:, ind] .= line
end

using Plots

anim = @animate for i in 1:150
    heatmap(world; axis = nothing, border = :none, cbar = false, ratio = :equal)
    evolve!(world)
end
gif(anim, "gameoflife.gif"; fps = 10)