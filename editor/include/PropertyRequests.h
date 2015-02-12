#ifndef __PROPERTY_REQUESTS_INCLUDED__
#define __PROPERTY_REQUESTS_INCLUDED__

#include <game/property.h>
#include <QDialog>

struct readTile : public game::property_read_visitor
{
    const char * name() override { return "Tile"; }

    game::point get_tile() const
    {
        return tile;
    }

    void visit(const game::property_point & prop) const override
    {
        tile = prop.get_value();
    }

private:
    mutable game::point tile;
};

struct readUniqueness : public game::property_read_visitor
{
    readUniqueness() : uniqueness(false) {}

    const char * name() override { return "Uniqueness"; }

    bool is_uniqueness() const
    {
        return uniqueness;
    }

    void visit(const game::property_bool & prop) const override
    {
        uniqueness = prop.get_value();
    }

private:
    mutable bool uniqueness;
};

struct readPosition : public game::property_read_visitor
{
    const char * name() override { return "Position"; }

    game::point get_position() const
    {
        return position;
    }

    void visit(const game::property_point & prop) const override
    {
        position = prop.get_value();
    }

private:
    mutable game::point position;
};

struct writePosition : public game::property_write_visitor
{
    const char * name() override { return "Position"; }

    writePosition(const game::point & pos)
        : position(pos) {}

    void visit(game::property_point & prop) override
    {
        prop.set_value(position);
    }

private:
    game::point position;
};

struct supportPosition : public game::property_read_visitor
{
    const char * name() override { return "Position"; }

    supportPosition() : support(false) {}

    bool supported() const
    {
        return support;
    }

    void visit(const game::property_point & prop) const override
    {
        support = true;
    }

private:
    mutable bool support;
};

#endif //__PROPERTY_REQUESTS_INCLUDED__
