#include <CommandExceptions.h>
#include <JsonExceptions.h>
#include <gtest/gtest.h>
#include "JsonObject.h"
#include "CommandParser.h"

class JsonTest : public testing::Test {
public:
    std::string json_str = R"({"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}})";

    JsonObject *json_object = nullptr;

    void SetUp() override {
        json_object = new JsonObject(json_str);
    }

    void TearDown() override {
        delete json_object;
    }
};

TEST_F(JsonTest, TestTrivial1) {
    std::string command = "a.b[1]";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    EXPECT_TRUE(std::holds_alternative<long>(value.get()));
    EXPECT_EQ(std::get<long>(value.get()), 2);
}

TEST_F(JsonTest, TestTrivial2) {
    std::string command = "a.b[2].c";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    EXPECT_TRUE(std::holds_alternative<std::string>(value.get()));
    EXPECT_EQ(std::get<std::string>(value.get()), "test");
}

TEST_F(JsonTest, TestTrivial3) {
    std::string command = "a.b";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<JsonArray>(value.get()));

    auto arr = std::get<JsonArray>(value.get()).get();

    ASSERT_EQ(arr.size(), 4);

    ASSERT_TRUE(std::holds_alternative<long>(arr[0].get()));
    ASSERT_TRUE(std::holds_alternative<long>(arr[1].get()));
    ASSERT_TRUE(std::holds_alternative<JsonObject>(arr[2].get()));

    ASSERT_EQ(std::get<long>(arr[0].get()), 1);
    ASSERT_EQ(std::get<long>(arr[1].get()), 2);

    auto map = std::get<JsonObject>(arr[2].get()).get();

    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.begin()->first, "c");

    ASSERT_TRUE(std::holds_alternative<std::string>(map.begin()->second.get()));
    ASSERT_EQ(std::get<std::string>(map.begin()->second.get()), "test");

    ASSERT_TRUE(std::holds_alternative<JsonArray>(arr[3].get()));

    auto arr1 = std::get<JsonArray>(arr[3].get()).get();

    ASSERT_EQ(arr1.size(), 2);

    ASSERT_TRUE(std::holds_alternative<long>(arr1[0].get()));
    ASSERT_TRUE(std::holds_alternative<long>(arr1[1].get()));
    ASSERT_EQ(std::get<long>(arr1[0].get()), 11);
    ASSERT_EQ(std::get<long>(arr1[1].get()), 12);
}

TEST_F(JsonTest, TestExpressionsInSubscript1) {
    std::string command = "a.b[a.b[1]].c";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<std::string>(value.get()));
    ASSERT_EQ(std::get<std::string>(value.get()), "test");
}

TEST_F(JsonTest, TestExpressionsInSubscript2) {
    std::string command = "a.b[max(1, 2)].c";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<std::string>(value.get()));
    ASSERT_EQ(std::get<std::string>(value.get()), "test");
}

TEST_F(JsonTest, TestExpressionsInSubscript3) {
    std::string command = "a.b[min(2, 3)].c";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<std::string>(value.get()));
    ASSERT_EQ(std::get<std::string>(value.get()), "test");
}

TEST_F(JsonTest, TestExpressionsInSubscript4) {
    std::string command = "a.b[size(a.b[3])].c";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<std::string>(value.get()));
    ASSERT_EQ(std::get<std::string>(value.get()), "test");
}

TEST_F(JsonTest, TestTripleArraySubscript) {
    JsonObject json_object1 = JsonObject(R"({"a": { "b": [ 1, 2, { "c": "test" }, [11, 12, [13, 14]] ]}})");

    std::string command = "a.b[3][2][1]";
    CommandParser command_parser(command, json_object1);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 14);
}

TEST_F(JsonTest, MinTest1) {
    std::string command = "min(a.b[0], a.b[1])";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 1);
}

TEST_F(JsonTest, MinTest2) {
    std::string command = "min(a.b[3])";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 11);
}

TEST_F(JsonTest, MinTest3) {
    std::string command = "min(a.b[0], 10, a.b[1], 15)";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 1);
}

TEST_F(JsonTest, MaxTest1) {
    std::string command = "max(a.b[0], a.b[1])";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 2);
}

TEST_F(JsonTest, MaxTest2) {
    std::string command = "max(a.b[3])";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 12);
}

TEST_F(JsonTest, MaxTest3) {
    std::string command = "max(a.b[0], 10, a.b[1], 15)";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 15);
}

TEST_F(JsonTest, SizeTest1) {
    std::string command = "size(a)";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 1);
}

TEST_F(JsonTest, SizeTest2) {
    std::string command = "size(a.b)";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 4);
}

TEST_F(JsonTest, SizeTest3) {
    std::string command = "size(a.b[a.b[1]].c)";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ASSERT_TRUE(std::holds_alternative<long>(value.get()));
    ASSERT_EQ(std::get<long>(value.get()), 4);
}

TEST_F(JsonTest, PrintTest) {
    std::stringstream ss;

    std::string command = "a.b";
    CommandParser command_parser(command, *json_object);
    auto value = command_parser.execute();

    ss << value;

    EXPECT_EQ(ss.str(), R"([1, 2, {"c": "test"}, [11, 12]])");
}

TEST_F(JsonTest, JsonError) {
    std::string err_msg;
    bool is_correct_exception = false;

    try {
        JsonObject json_object1 = JsonObject(R"({"a": { "b": [ 1, F2, { "c": "test" }, [11, 12, [13, 14]] ]}})");
    } catch (JsonException &e) {
        err_msg = e.what();
        is_correct_exception = dynamic_cast<JsonUnexpectedTokenException *>(&e) != nullptr;
    }

    EXPECT_TRUE(err_msg.find("18") != std::string::npos);
    EXPECT_TRUE(is_correct_exception);
}

TEST_F(JsonTest, CommandLexError) {
    std::string command = "a.b.";
    std::string err_msg;
    bool is_correct_exception = false;

    try {
        CommandParser command_parser(command, *json_object);
        auto value = command_parser.execute();
    } catch (CommandException &e) {
        err_msg = e.what();
        is_correct_exception = dynamic_cast<CommandUnexpectedTokenException *>(&e) != nullptr;
    }

    ASSERT_TRUE(err_msg.find('3') != std::string::npos);
    ASSERT_TRUE(is_correct_exception);
}

TEST_F(JsonTest, CommandParseError) {
    std::string command = "a.b[4]";
    std::string err_msg;
    bool is_correct_exception = false;

    try {
        CommandParser command_parser(command, *json_object);
        auto value = command_parser.execute();
    } catch (CommandException &e) {
        err_msg = e.what();
        is_correct_exception = dynamic_cast<CommandArrayIndexOutOfRange *>(&e) != nullptr;
    }

    ASSERT_TRUE(err_msg.find('4') != std::string::npos);
    ASSERT_TRUE(is_correct_exception);
}
