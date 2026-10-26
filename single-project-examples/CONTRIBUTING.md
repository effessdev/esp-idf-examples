# Contributing

If you want to contribute to this repo by creating a new example, **thank you so much** 🥹 Let me make it as easy as possible for you!

> Please create an issue before contributing to make sure your work will be approved. The new example must be consistent with the rest, both in subject and difficulty.

## Setting up the development environment

Check out the following chapters from my [ESP-IDF Course](https://effessdev.github.io/esp-idf):

- [Installation](https://effessdev.github.io/esp-idf/getting-started/install/)
- [Your first project](https://effessdev.github.io/esp-idf/getting-started/first-project/)

## Steps

1. Write your example in `main/example_name.c`. Name the main function `example_name_main()`.
2. Create the header file in `main/include` directory.
3. Add `example_name.c` in `main/CMakeLists.txt`.
4. Add a link to the [Recommended learning order](README.md#recommended-learning-order) section in the README.

## Testing

Update the calling statement in `main.c`, build, and flash.
