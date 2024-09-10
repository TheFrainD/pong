local System = require "System"

local SPEED = 100.0
local dx = -SPEED
local dy = 0

function onCollision(other)
    dx = -dx * 1.03

    local transform = self.GetComponent("Transform")
    local playerPosition = other.GetComponent("Transform").position

    local name = other.GetName()
    if name == "Player1" then
        transform.position.x = playerPosition.x + other.GetComponent("Sprite").size.x
    else
        transform.position.x = playerPosition.x - self.GetComponent("Sprite").size.x
    end

    if dy < 0 then
        dy = -math.random(10, 150)
    else
        dy = math.random(10, 150)
    end
end

function update(deltaTime)
    local transform = self.GetComponent("Transform")

    transform.position.x = transform.position.x + dx * deltaTime

    if transform.position.x < 0 or transform.position.x > System.Window.GetWidth() then
        transform.position.x = System.Window.GetWidth() / 2.0
    end

    transform.position.y = transform.position.y + dy * deltaTime

    if transform.position.y < 0 or
            transform.position.y > System.Window.GetHeight() - self.GetComponent("Sprite").size.y then
        dy = -dy
    end
end
