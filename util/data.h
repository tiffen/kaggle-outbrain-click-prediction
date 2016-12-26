#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "io.h"

struct event {
    int document_id;
    int timestamp;
    int platform;
    std::string uuid;
    std::string country;
    std::string state;
    std::string region;
};

struct ad {
    int document_id;
    int campaign_id;
    int advertiser_id;
};

struct document {
    int source_id;
    int publisher_id;
};


// Functions to read data types


std::pair<int, event> read_event(const std::vector<std::string> & row) {
    event e;

    e.document_id = stoi(row[2]);
    e.timestamp = stoi(row[3]);

    try {
        e.platform = stoi(row[4]);
    } catch (std::invalid_argument) {
        e.platform = 0;
    }

    e.uuid = row[1];
    e.country = row[5];
    e.state = row[6];
    e.region = row[7];

    return std::make_pair(stoi(row[0]), e);
}

std::pair<int, ad> read_ad(const std::vector<std::string> & row) {
    ad a;

    a.document_id = stoi(row[1]);
    a.campaign_id = stoi(row[2]);
    a.advertiser_id = stoi(row[3]);

    return std::make_pair(stoi(row[0]), a);
}

std::pair<int, document> read_document(const std::vector<std::string> & row) {
    document d;

    d.source_id = stoi(row[1]);
    d.publisher_id = stoi(row[2]);

    return std::make_pair(stoi(row[0]), d);
}

std::pair<int, std::pair<int, float>> read_document_annotation(const std::vector<std::string> & row) {
    return std::make_pair(stoi(row[0]), std::make_pair(stoi(row[1]), stof(row[2])));
}


std::vector<event> read_events() {
    return read_vector("cache/events.csv.gz", read_event, 23120127);
}

std::vector<ad> read_ads() {
    return read_vector("../input/promoted_content.csv.gz", read_ad, 573099);
}


// All data


struct reference_data {
    std::vector<event> events;
    std::vector<ad> ads;
    std::unordered_map<int, document> documents;
    std::unordered_multimap<int, std::pair<int, float>> document_categories;
    std::unordered_multimap<int, std::pair<int, float>> document_topics;
    std::unordered_multimap<int, std::pair<int, float>> document_entities;
};

reference_data load_reference_data() {
    reference_data res;
    res.events = read_events();
    res.ads = read_ads();
    res.documents = read_map("cache/documents.csv.gz", read_document);
    res.document_categories = read_multi_map("../input/document_categories.csv.gz", read_document_annotation);
    res.document_topics = read_multi_map("../input/document_topics.csv.gz", read_document_annotation);
    res.document_entities = read_multi_map("cache/document_entities.csv.gz", read_document_annotation);

    return res;
}
