/*struct NewMove {
	int i;
	std::string from;
	std::string to;
};

NewMove event_move_started(const int figure, const Vector2f& position) {
	std::string start_note = toChessNote(position);
	return NewMove{
		figure,
		start_note,
		""
	};
}

void event_move_finished(NewMove& move, const int figure, const Vector2f& position)
{
	const auto end_note = toChessNote(position);
	move.to = end_note;
}

void newMove()
{
	for (int i = 0; i < 32; i++)
		if (figures[i].getGlobalBounds().contains(pos.x, pos.y))
		{
			isMove = true;

			n = i;
			dx = pos.x - figures[i].getPosition().x;
			dy = pos.y - figures[i].getPosition().y;
			oldPosition = figures[i].getPosition();
		}
}*/