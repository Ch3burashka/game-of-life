#pragma once
#include <array>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

class LifeException : public std::runtime_error
{
  public:
	using std::runtime_error::runtime_error;
};

static std::string backup_name = "state.bin";

class Life
{
  public:
	Life(int width, int height);

	const std::vector<bool>& GetCellsStates() const;
	void FillRandomStates();
	explicit operator std::string() const;

	friend std::ostream& operator<<(std::ostream& os, const Life& life);

	void SetStateAlive(int x, int y);

	void SetStateDead(int x, int y);

	bool GetStateCell(int x, int y) const;

	int GetLiveCountNearCell(int x, int y) const;

	void Mutate();

	int GetWidth() const;

	int GetHeight() const;

	void SaveState(const std::string& path) const;

	void LoadState(const std::string& path);

	void ClearStates();
  private:
	int width_;
	int height_;
	std::vector<bool> cells_states_;
	std::string executable_path_;
	bool IsValid_(int x, int y) const;

	std::vector<bool> GetNearStates_(int x, int y) const;
};
