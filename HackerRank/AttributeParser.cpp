#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <stack>

#pragma warning(disable:4996)
using namespace std;

class Tag
{
public:
    map<string, string> attributes;
    map<string, Tag*> children;

    ~Tag()
    {
        for (auto iter = children.begin(); iter != children.end(); iter++)
            delete iter->second;
    }
};

int N, Q;
map<string, Tag*> root_tags;
stack<Tag*> tag_stack;

string ParseTagName(char* source, int& index)
{
    index = 1;
    while (source[index] != ' ' && source[index] != '>')
        index++;
    source[index++] = 0;
    return string(source + 1);
}

void ParseAttributesFrom(Tag* tag, char* source, int index)
{
    while (source[index] != 0)
    {
        int name_start, value_start;
        name_start = index;

        while (source[index] != ' ')
            index++;
        source[index++] = 0;

        while (source[index] != ' ')
            index++;
        index += 2;
        value_start = index;

        while (source[index] != '\"')
            index++;
        source[index] = 0;
        index += 2;

        tag->attributes[string(source + name_start)] = string(source + value_start);
    }
}

void ParseOpeningTag(char* source)
{
    int index;
    Tag* new_tag = new Tag();
    string tag_name = ParseTagName(source, index);

    ParseAttributesFrom(new_tag, source, index);

    if (tag_stack.empty())
        root_tags[string(source + 1)] = new_tag;
    else
        tag_stack.top()->children[tag_name] = new_tag;
    tag_stack.push(new_tag);
}

void ParseEndTag()
{
    tag_stack.pop();
}

void ParseSource(char* source)
{
    if (source[1] == '/')
        ParseEndTag();
    else
        ParseOpeningTag(source);
}

string ParseReference(char* query)
{
    int start_index, next_index = 0;
    Tag* parent_tag = nullptr;

    while (true)
    {
        start_index = next_index;
        while (query[next_index] != '.' && query[next_index] != '~')
            next_index++;

        bool next_is_tag = (query[next_index] == '.');
        query[next_index++] = 0;

        map<string, Tag*>* name_tag_map = parent_tag == nullptr ? &root_tags : &parent_tag->children;
        auto target_tag_iter = name_tag_map->find(string(query + start_index));
        if (target_tag_iter == name_tag_map->end())
            return string("Not Found!");
        else
            parent_tag = target_tag_iter->second;

        if (!next_is_tag)
        {
            auto attr = parent_tag->attributes.find(string(query + next_index));
            if (attr == parent_tag->attributes.end())
                return string("Not Found!");
            else
                return attr->second;
        }
    }
}

int main() {
    scanf("%d%d", &N, &Q);
    cin.get();

    char buf[201];
    for (int i = 0; i < N; ++i)
    {
        cin.getline(buf, 205);
        ParseSource(buf);
    }

    for (int i = 0; i < Q; ++i)
    {
        cin.getline(buf, 201);
        cout << ParseReference(buf) << "\n";
    }

    for (auto iter = root_tags.begin(); iter != root_tags.end(); iter++)
        delete iter->second;

    return 0;
}
