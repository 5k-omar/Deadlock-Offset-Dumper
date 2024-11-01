# DeadLock Offsets Dumber

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![C++](https://img.shields.io/badge/Language-C++-blue)
<p align="center"> <img src="https://komarev.com/ghpvc/?username=5k-omar&label=Repo%20views&color=0e75b6&style=flat" alt="Repo Views" /> </p>

**My Discord Server**: [Join Here](https://discord.gg/tcnksFMCR9)

DeadLock Offsets Dumber is a lightweight C++ tool for extracting critical game offsets for **DeadLock**. Given the frequent game updates, offsets may change every few hours, and this tool helps you always have the latest values.

> **Note:** This tool is provided for educational purposes only. Use it responsibly and ensure it aligns with any applicable terms of service.

## 📜 Table of Contents
- [Features](#features)
- [How to Get DeadLock Offsets](#how-to-get-deadlock-offsets)
- [Offsets List](#offsets-list)
- [Requirements](#requirements)
- [Usage](#usage)
- [FAQ](#faq)
- [License](#license)

---

## 🎯 Features

- **Automated Offset Retrieval**: Keeps all essential game offsets up-to-date.
- **Compatible with Any Version**: Works across different game versions without additional configuration.
- **Simple, User-Friendly Interface**: Minimalistic design for quick access to tools and updates.

---

## 🛠 How to Get DeadLock Offsets

Since the game updates every few hours, you’ll need to retrieve fresh offsets regularly. Follow these steps:

1. **Run the DeadLock Offsets Dumber Tool**: See [Usage](#usage) for instructions.
2. **Validate Offset Accuracy**: If offsets aren’t working as expected, re-run the tool or wait for the next patch.
3. **Apply Offsets**: After retrieval, use these offsets in your project as required.

---

## 📝 Offsets List

The following offsets are supported in the current version of DeadLock Offsets Dumber:

```plaintext
1. dwLocalPlayerController - (LocalPlayer)
2. dwViewMatrix
3. dwEntityList
4. dwGameEntitySystem
5. dwViewRender
6. CCameraManager
7. SchemaSystemInterface
```

### Sample Output

Below is an example of the retrieved offsets output:

```plaintext
dwLocalPlayerController: 0xDEADBEEF
dwViewMatrix: 0xBAADF00D
dwEntityList: 0xB16B00B5
dwGameEntitySystem: 0xC0FFEE00
```


## ❓ FAQ
### 1. **Why do offsets change frequently?**
   Offsets are tied to the game’s internal memory structure, which shifts with each update to prevent unauthorized manipulation.

### 2. **What happens if my offsets stop working?**
   Re-run the tool to retrieve updated offsets. This is normal, especially after a patch.

### 3. **Can I Get ban by using this tool**
   No, Game Not Have anticheats

## 📜 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the [issues page](https://github.com/5k-omar/Deadlock-Offset-Dumper/issues) or create a pull request.

**Made with ❤️ by [5komar (Catrine)](https://github.com/5k-omar)**
