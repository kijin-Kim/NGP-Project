#pragma once

class State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void Render() = 0;
};


