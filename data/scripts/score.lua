Score = {
	player_one = 0,
	player_two = 0,
}

local labels = {
	player_one = nil,
	player_two = nil,
}

function OnStart()
	labels.player_one = GetEntity("Player1Score").GetComponent("Label")
	labels.player_two = GetEntity("Player2Score").GetComponent("Label")
end

function SetPlayerOneScore(score)
	Score.player_one = score
	labels.player_one.text = tostring(score)
end

function SetPlayer2Score(score)
	Score.player_two = score
	labels.player_two.text = tostring(score)
end
