local Input = require("Input")

local objects = {
	score_manager = nil,
	player_one = nil,
	player_two = nil,
	ball = nil,
}

State = {}

function State:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function State:enter(deltaTime) end

function State:exit(deltaTime) end

function State:update(deltaTime) end

ServingState = State:new()

function ServingState:enter()
	local direction = -1
	if self.serve == 2 then
		direction = 1
	end
	objects.ball.Serve(direction)
end

function ServingState:update(deltaTime)
	if Input.IsKeyDown(Input.Key.Space) then
		ChangeState(PlayingState:new())
	end
end

PlayingState = State:new()

function PlayingState:enter()
	objects.score_manager.Reset()
end

function PlayingState:update(deltaTime)
	objects.player_one.update(deltaTime)
	objects.player_two.update(deltaTime)
	objects.ball.update(deltaTime)
end

local currentState = nil

function Init()
	objects.score_manager = GetEntity("ScoreManager").GetComponent("Score")
	objects.player_one = GetEntity("Player1").GetComponent("Player")
	objects.player_two = GetEntity("Player2").GetComponent("Player")
	objects.ball = GetEntity("Ball").GetComponent("Ball")
end

function OnStart()
	ChangeState(ServingState:new({ serve = 1 }))
end

function Update(deltaTime)
	if currentState then
		currentState:update(deltaTime)
	end
end

function ChangeState(state)
	if currentState then
		currentState:exit()
	end
	currentState = state
	currentState:enter()
end
