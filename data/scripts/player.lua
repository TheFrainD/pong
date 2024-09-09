local Input = require "Input"
local System = require "System"

function update(deltaTime)
    local speed = 250.0

    local transform = GetComponent("Transform")
    local sprite = GetComponent("Sprite")

    if not transform or not sprite then
        return
    end

    local upKey
    local downKey

    if ctx.isPlayerOne then
        upKey = Input.Key.W
        downKey = Input.Key.S
    else
        upKey = Input.Key.Up
        downKey = Input.Key.Down
    end

    local position = transform.position

    if Input.IsKeyDown(upKey) and position.y > 0 then
        position.y = position.y - speed * deltaTime;
    elseif Input.IsKeyDown(downKey) and position.y < (System.Window.GetHeight() - sprite.size.y) then
        position.y = position.y + speed * deltaTime;
    end
end