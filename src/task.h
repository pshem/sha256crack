#pragma once

/* Abstract base class: a task to be executed. */
class Task
{
public:
	virtual ~Task()
	{
	}

	/* Perform the task. Subclasses must override this. */
	virtual void run() = 0;
};

