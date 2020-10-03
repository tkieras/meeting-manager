package com.tkieras.eecs381.project0;

import java.util.*;


class WithMessage<T> {
	
	private final T object;

	private final String message;

	public WithMessage() {
		this.object = null;
		this.message = null;
	}

	public WithMessage(T obj) {
		this.object = obj;
		this.message = null;
	}

	public WithMessage(T obj, String message) {
		this.object = obj;
		this.message = message;
	}

	public WithMessage(String message) {
		this.object = null;
		this.message = message;
	}

	public boolean hasObject() {
		return this.object != null;
	}

	public boolean hasMessage() {
		return this.message != null;
	}

	public T getObject() {
		return this.object;
	}

	public String getMessage() {
		return this.message;
	}

	public WithMessage<T> collectMessage(WithMessage other) {
		if (this.hasMessage()) {
			return this;
		}

		return new WithMessage<T>(other.getMessage());
	}

}