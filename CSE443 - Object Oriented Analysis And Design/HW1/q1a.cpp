// Base class for media files
class Media {
public:
    virtual ~Media() = default;
    virtual void accept(class MediaVisitor& visitor) = 0;
};

// Derived class for audio files
class Audio : public Media {
public:

    // Accept visitor
    void accept(MediaVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// Derived class Video
class Video : public Media {
public:

    void accept(MediaVisitor& visitor) override {
        visitor.visit(*this);
    }
};


// Visitor interface for new operations
class MediaVisitor {
public:
    virtual void visit(Audio& audio) = 0;
    virtual void visit(Video& video) = 0;

    virtual ~MediaVisitor() = default;
};

class Play : public MediaVisitor {  
public:
    void visit(Audio& audio) override {
        // Play audio
    }

    void visit(Video& video) override {
        // Play video
    }
};

class Filter : public MediaVisitor {
public:
    void visit(Audio& audio) override {
        // Filter audio
    }

    void visit(Video& video) override {
        // Filter video
    }
};

class Export : public MediaVisitor {
public:
    void visit(Audio& audio) override {
        // Export audio
    }

    void visit(Video& video) override {
        // Export video
    }
};



int main() {
    std::vector<std::unique_ptr<Media>> media;
    media.emplace_back(std::make_unique<Audio>());
    media.emplace_back(std::make_unique<Video>());

    Filter filterMedia;
    Export exportMedia;
    Play playMedia;

    // Apply filter and export to all media
    for (auto& m : media) {
        m->accept(filterMedia);
        m->accept(exportMedia);
        m->accept(playMedia);
    }
}




