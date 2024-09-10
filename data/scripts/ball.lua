local System = require "System"

local speed = 100.0
local hvelocity = -speed

function onCollision(other)
    local name = other.GetName()

    if name == "Player1" then
        hvelocity = speed
    elseif name == "Player2" then
        hvelocity = -speed
    end
end

function update(deltaTime)
    local transform = self.GetComponent("Transform")

    transform.position.x = transform.position.x + hvelocity * deltaTime

    if transform.position.x < 0 or transform.position.x > System.Window.GetWidth() then
        transform.position.x = System.Window.GetWidth() / 2.0
    end
end
