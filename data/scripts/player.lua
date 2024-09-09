local Input = require("Input")

function update(deltaTime)
    local speed = 200.0
    local transform = GetComponent("Transform")
    if not transform then
        return
    end

    if Input.IsKeyDown(Input.Key.W) then
        transform.position.y = transform.position.y - speed * deltaTime;
    elseif Input.IsKeyDown(Input.Key.S) then
        transform.position.y = transform.position.y + speed * deltaTime;
    end
end