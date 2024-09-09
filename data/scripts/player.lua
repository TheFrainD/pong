local Input = require("Input")

function onStart()
    print("Hello, Pong!")
end

function update(deltaTime)
    if Input.IsKeyDown(Input.Key.Space) then
        print("Space is down!")
    end
end