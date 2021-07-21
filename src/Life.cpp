#include "Life.h"
#include <array>
#include <fstream>

namespace
{
	std::ostream& serialize(std::ostream& os, const std::vector<bool> v)
	{
		auto size = v.size();
		std::vector<uint8_t> res;
		res.reserve(size);

		for (const auto& item : v)
		{
			if (item)
			{
				res.push_back(1);
			}
			else
			{
				res.push_back(0);
			}
		}
		os.write(reinterpret_cast<char const*>(&size), sizeof(size));
		os.write(reinterpret_cast<char const*>(res.data()), res.size() * sizeof(uint8_t));
		return os;
	}

	std::istream& deserialize(std::istream& is, std::vector<bool>& v)
	{
		std::vector<uint8_t> nums;
		auto size = v.size();
		nums.resize(size);
		is.read(reinterpret_cast<char*>(&size), sizeof(size));
		is.read(reinterpret_cast<char*>(nums.data()), nums.size() * sizeof(uint8_t));

		for (int i = 0; i < size; ++i)
		{
			if (nums[i] == static_cast<uint8_t>(1))
			{
				v[i] = true;
			}
			else
			{
				v[i] = false;
			}
		}
		return is;
	}

} // namespace

Life::Life(int width, int height) : width_(width), height_(height), cells_states_(std::vector<bool>(width * height))
{
	if (width < 3 || height < 3)
	{
		throw LifeException("Wrong size");
	}
}

const std::vector<bool>& Life::GetCellsStates() const
{
	return cells_states_;
}

Life::operator std::string() const
{
	using namespace std;

	std::string result;
	size_t buf_size = 0;
	for (size_t y = 0; y < height_; ++y)
	{
		std::string buffer = "|";
		size_t counter = 0;
		for (size_t x = 0; x < width_; ++x)
		{
			std::string item = (cells_states_[(y * width_) + x]) ? "o" : "x";
			++counter;
			if (counter == width_)
			{
				buffer += item;
			}
			else
			{
				buffer += item + " ";
			}
		}
		buffer += "|";
		buf_size = buffer.size();
		result += buffer + "\n";
	}
	return std::string(buf_size, '.') + "\n" + result + std::string(buf_size, '.');
}

std::ostream& operator<<(std::ostream& os, const Life& life)
{
	os << std::string(life);
	return os;
}

void Life::SetStateAlive(int x, int y)
{
	using namespace std;
	if (IsValid_(x, y))
	{
		cells_states_[(y * width_) + x] = true;
	}
	else
	{
		throw LifeException("Wrong pos ("s + std::to_string(x) + ", " + std::to_string(y) + ")");
	}
}

void Life::SetStateDead(int x, int y)
{
	using namespace std;
	if (IsValid_(x, y))
	{
		cells_states_[(y * width_) + x] = false;
	}
	else
	{
		throw LifeException("Wrong pos ("s + std::to_string(x) + ", " + std::to_string(y) + ")");
	}
}

bool Life::GetStateCell(int x, int y) const
{
	using namespace std;
	if (x == -1)
	{
		x = x + width_;
	}
	if (y == -1)
	{
		y = y + height_;
	}
	if (x == width_)
	{
		x = 0;
	}
	if (y == height_)
	{
		y = 0;
	}

	if (IsValid_(x, y))
	{
		return cells_states_[(y * width_) + x];
	}
	else
	{
		throw LifeException("Wrong pos ("s + std::to_string(x) + ", " + std::to_string(y) + ")");
	}
}

bool Life::IsValid_(int x, int y) const
{
	return x >= 0 && y >= 0 && x < width_ && y < height_;
}

std::vector<bool> Life::GetNearStates_(int x, int y) const
{
	using namespace std;
	if (IsValid_(x, y))
	{
		return {
			GetStateCell(x - 1, y - 1), GetStateCell(x, y - 1),		GetStateCell(x + 1, y - 1),
			GetStateCell(x - 1, y),		GetStateCell(x + 1, y),		GetStateCell(x - 1, y + 1),
			GetStateCell(x, y + 1),		GetStateCell(x + 1, y + 1),
		};
	}
	else
	{
		throw LifeException("Wrong pos ("s + std::to_string(x) + ", " + std::to_string(y) + ")");
	}
}

int Life::GetLiveCountNearCell(int x, int y) const
{
	auto near_cells = GetNearStates_(x, y);
	return std::count(near_cells.begin(), near_cells.end(), true);
}

void Life::Mutate()
{
	Life other(this->width_, this->height_);

	for (int x = 0; x < width_; ++x)
	{
		for (int y = 0; y < height_; ++y)
		{
			if (!GetStateCell(x, y))
			{
				if (GetLiveCountNearCell(x, y) == 3)
				{
					other.SetStateAlive(x, y);
				}
				else
				{
					continue;
				}
			}
			else
			{
				if (GetLiveCountNearCell(x, y) == 3 || GetLiveCountNearCell(x, y) == 2)
				{
					other.SetStateAlive(x, y);
				}
				else
				{

					other.SetStateDead(x, y);
				}
			}
		}
	}
	std::swap(*this, other);
}
int Life::GetWidth() const
{
	return width_;
}
int Life::GetHeight() const
{
	return height_;
}
void Life::FillRandomStates()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10000);
	std::generate(cells_states_.begin(), cells_states_.end(), [&]() { return dist6(rng) % 2 == 0; });
}

void Life::SaveState(const std::string& path) const
{
	std::ofstream file(path, std::ios::binary | std::ios::out);
	::serialize(file, cells_states_);
	file.close();
}

void Life::LoadState(const std::string& path)
{
	std::ifstream ifile(path, std::ios::binary | std::ios::in);
	::deserialize(ifile, cells_states_);
	ifile.close();
}

void Life::ClearStates()
{
	std::fill(cells_states_.begin(), cells_states_.end(), false);
}
