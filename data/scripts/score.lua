player1Score = 0
player2Score = 0

local player1ScoreLabel
local player2ScoreLabel

function OnStart()
	player1ScoreLabel = GetEntity("Player1Score").GetComponent("Label")
	player2ScoreLabel = GetEntity("Player2Score").GetComponent("Label")
end

function SetPlayer1Score(score)
	player1Score = score
	player1ScoreLabel.text = tostring(score)
end

function SetPlayer2Score(score)
	player2Score = score
	player2ScoreLabel.text = tostring(score)
end
