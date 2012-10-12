namespace utils {
    // logics: fixed-point ordering is just integer ordering
    _tmpl_head_
    bool _cls_name_::operator <(_cls_name_ const& x) const
    {
        return this->value() < x.value();
    }

    _tmpl_head_
    bool _cls_name_::operator ==(_cls_name_ const& x) const
    {
        return this->value() == x.value();
    }

    _tmpl_head_
    bool _cls_name_::operator !() const
    {
        return this->value() == 0;
    }
}
