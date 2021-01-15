#include "fundot_eval.h"

namespace fundot {

Object if_(Evaluator* eval, const List& list)
{
    auto iter = ++list.value.begin();
    Object result = (*eval)(*iter);
    if (result.value.type() == typeid(Null)
        || result == Object({Boolean({false})})) {
        if (++iter != list.value.end() && ++iter != list.value.end()) {
            return (*eval)(*iter);
        }
        return {Null()};
    }
    if (++iter != list.value.end()) {
        return (*eval)(*iter);
    }
    return {Null()};
}

Object cond(Evaluator* eval, const List& list)
{
    auto iter = list.value.begin();
    while (++iter != list.value.end()) {
        Object result = (*eval)(*iter);
        ++iter;
        if (result.value.type() != typeid(Null)
            && result != Object({Boolean({false})})) {
            return (*eval)(*iter);
        }
    }
    return {Null()};
}

Object while_(Evaluator* eval, const List& list)
{
    auto iter = ++list.value.begin();
    Object predicate = *iter;
    Object to_eval = *++iter;
    Object result = (*eval)(predicate);
    Object ret({Null()});
    while (result.value.type() != typeid(Null)
           && result != Object({Boolean({false})})) {
        ret = (*eval)(to_eval);
        result = (*eval)(predicate);
    }
    return ret;
}

Object lambda(Evaluator*, const List& list)
{
    if (list.value.size() < 3) {
        return {Null()};
    }
    auto iter = ++list.value.begin();
    if (iter->value.type() != typeid(Vector)) {
        return {Null()};
    }
    Function function;
    function.params = std::any_cast<const Vector&>(iter->value);
    function.body = *++iter;
    return {function};
}

Object defun(Evaluator* eval, const List& list)
{
    if (list.value.size() < 4) {
        return {Null()};
    }
    auto iter = ++list.value.begin();
    Setter setter;
    setter.value.first = *iter;
    if ((++iter)->value.type() != typeid(Vector)) {
        return {Null()};
    }
    Function function;
    function.params = std::any_cast<const Vector&>(iter->value);
    function.body = *++iter;
    setter.value.second = {function};
    return (*eval)({setter});
}

Object quit(Evaluator*, const List&)
{
    exit(EXIT_SUCCESS);
}

Object eval_(Evaluator* eval, const List& list)
{
    if (list.value.size() < 2) {
        return {Null()};
    }
    return (*eval)(*++list.value.begin());
}

Object scan(Evaluator*, const List&)
{
    Object object;
    std::cin >> object;
    return object;
}

Object print(Evaluator*, const List& list)
{
    if (list.value.size() > 1) {
        std::cout << *++list.value.begin() << '\n';
    }
    return {Void()};
}

Object Evaluator::operator()(const Object& object)
{
    return eval(object);
}

Object Evaluator::global(const List&)
{
    return scope_;
}

Object Evaluator::eval(const Adder& adder)
{
    return eval(adder.value.first) + eval(adder.value.second);
}

Object Evaluator::eval(const Subtractor& subtractor)
{
    return eval(subtractor.value.first) - eval(subtractor.value.second);
}

Object Evaluator::eval(const Multiplier& multiplier)
{
    return eval(multiplier.value.first) * eval(multiplier.value.second);
}

Object Evaluator::eval(const Divisor& divisor)
{
    return eval(divisor.value.first) / eval(divisor.value.second);
}

Object Evaluator::eval(const Modular& modular)
{
    return eval(modular.value.first) % eval(modular.value.second);
}

Object Evaluator::eval(const Negator& negator)
{
    return -eval(negator.value);
}

Object Evaluator::eval(const Less& less)
{
    return {Boolean({eval(less.value.first) < eval(less.value.second)})};
}

Object Evaluator::eval(const Greater& greater)
{
    return {Boolean({eval(greater.value.first) > eval(greater.value.second)})};
}

Object Evaluator::eval(const LessEqual& less_equal)
{
    return {Boolean(
        {eval(less_equal.value.first) <= eval(less_equal.value.second)})};
}

Object Evaluator::eval(const GreaterEqual& greater_equal)
{
    return {Boolean(
        {eval(greater_equal.value.first) >= eval(greater_equal.value.second)})};
}

Object Evaluator::eval(const EqualTo& equal_to)
{
    return {
        Boolean({eval(equal_to.value.first) == eval(equal_to.value.second)})};
}

Object Evaluator::eval(const NotEqualTo& not_equal_to)
{
    return {Boolean(
        {eval(not_equal_to.value.first) != eval(not_equal_to.value.second)})};
}

Object Evaluator::eval(const And& logical_and)
{
    return eval(logical_and.value.first) && eval(logical_and.value.second);
}

Object Evaluator::eval(const Or& logical_or)
{
    return eval(logical_or.value.first) || eval(logical_or.value.second);
}

Object Evaluator::eval(const Not& logical_not)
{
    return !eval(logical_not.value);
}

Object Evaluator::eval(const BitwiseAnd& bitwise_and)
{
    return eval(bitwise_and.value.first) & eval(bitwise_and.value.second);
}

Object Evaluator::eval(const BitwiseOr& bitwise_or)
{
    return eval(bitwise_or.value.first) | eval(bitwise_or.value.second);
}

Object Evaluator::eval(const BitwiseXor& bitwise_xor)
{
    return eval(bitwise_xor.value.first) ^ eval(bitwise_xor.value.second);
}

Object Evaluator::eval(const BitwiseNot& bitwise_not)
{
    return ~eval(bitwise_not.value);
}

Object Evaluator::eval(const LeftShift& left_shift)
{
    return eval(left_shift.value.first) << eval(left_shift.value.second);
}

Object Evaluator::eval(const RightShift& right_shift)
{
    return eval(right_shift.value.first) >> eval(right_shift.value.second);
}

Object Evaluator::eval(const Symbol& symbol)
{
    Object* obj_ptr = get(scope_, {symbol});
    if (obj_ptr != nullptr) {
        return eval(*obj_ptr);
    }
    return {symbol};
}

Object Evaluator::eval(const Getter& getter)
{
    Object* obj_ptr = get(scope_, {getter});
    if (obj_ptr != nullptr) {
        return eval(*obj_ptr);
    }
    return {Null()};
}

Object Evaluator::eval(const Setter& setter)
{
    set(scope_, setter.value.first, eval(setter.value.second));
    Object* obj_ptr = get(scope_, setter.value.first);
    if (obj_ptr != nullptr) {
        return *obj_ptr;
    }
    return {Null()};
}

Object Evaluator::eval(const Assignment& assignment)
{
    return eval(
        {Setter({{eval(assignment.value.first), assignment.value.second}})});
}

Object Evaluator::eval(const List& list)
{
    static std::unordered_map<
        Object, std::function<Object(Evaluator*, const List&)>, Hash<Object>>
        built_in_macros = {{{Symbol({"if"})}, if_},
                           {{Symbol({"cond"})}, cond},
                           {{Symbol({"while"})}, while_},
                           {{Symbol({"lambda"})}, lambda},
                           {{Symbol({"defun"})}, defun}};
    if (built_in_macros.find(list.value.front()) != built_in_macros.end()) {
        return built_in_macros[list.value.front()](this, list);
    }
    List after_eval;
    for (const auto& elem : list.value) {
        after_eval.value.push_back(eval(elem));
    }
    static std::unordered_map<
        Object, std::function<Object(Evaluator*, const List&)>, Hash<Object>>
        built_in_functions = {{{Symbol({"global"})}, &Evaluator::global},
                              {{Symbol({"quit"})}, quit},
                              {{Symbol({"eval"})}, eval_},
                              {{Symbol({"scan"})}, scan},
                              {{Symbol({"print"})}, print}};
    if (built_in_functions.find(list.value.front())
        != built_in_functions.end()) {
        return built_in_functions[list.value.front()](this, after_eval);
    }
    if (after_eval.value.front().value.type() == typeid(Function)) {
        Function& function =
            std::any_cast<Function&>(after_eval.value.front().value);
        Evaluator local_eval;
        local_eval.scope_ = scope_;
        auto iter = after_eval.value.begin();
        for (const auto& arg : function.params.value) {
            if (++iter == after_eval.value.end()) {
                break;
            }
            local_eval.eval(Setter({{arg, *iter}}));
        }
        return local_eval(function.body);
    }
    return {after_eval.value.back()};
}

Object Evaluator::eval(const Quote& quote)
{
    return quote.value;
}

Object Evaluator::eval(const Object& object)
{
    if (object.value.type() == typeid(Adder)) {
        return eval(std::any_cast<const Adder&>(object.value));
    }
    if (object.value.type() == typeid(Subtractor)) {
        return eval(std::any_cast<const Subtractor&>(object.value));
    }
    if (object.value.type() == typeid(Multiplier)) {
        return eval(std::any_cast<const Multiplier&>(object.value));
    }
    if (object.value.type() == typeid(Divisor)) {
        return eval(std::any_cast<const Divisor&>(object.value));
    }
    if (object.value.type() == typeid(Modular)) {
        return eval(std::any_cast<const Modular&>(object.value));
    }
    if (object.value.type() == typeid(Negator)) {
        return eval(std::any_cast<const Negator&>(object.value));
    }
    if (object.value.type() == typeid(Less)) {
        return eval(std::any_cast<const Less&>(object.value));
    }
    if (object.value.type() == typeid(Greater)) {
        return eval(std::any_cast<const Greater&>(object.value));
    }
    if (object.value.type() == typeid(LessEqual)) {
        return eval(std::any_cast<const LessEqual&>(object.value));
    }
    if (object.value.type() == typeid(GreaterEqual)) {
        return eval(std::any_cast<const GreaterEqual&>(object.value));
    }
    if (object.value.type() == typeid(EqualTo)) {
        return eval(std::any_cast<const EqualTo&>(object.value));
    }
    if (object.value.type() == typeid(NotEqualTo)) {
        return eval(std::any_cast<const NotEqualTo&>(object.value));
    }
    if (object.value.type() == typeid(And)) {
        return eval(std::any_cast<const And&>(object.value));
    }
    if (object.value.type() == typeid(Or)) {
        return eval(std::any_cast<const Or&>(object.value));
    }
    if (object.value.type() == typeid(Not)) {
        return eval(std::any_cast<const Not&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseAnd)) {
        return eval(std::any_cast<const BitwiseAnd&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseOr)) {
        return eval(std::any_cast<const BitwiseOr&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseXor)) {
        return eval(std::any_cast<const BitwiseXor&>(object.value));
    }
    if (object.value.type() == typeid(BitwiseNot)) {
        return eval(std::any_cast<const BitwiseNot&>(object.value));
    }
    if (object.value.type() == typeid(LeftShift)) {
        return eval(std::any_cast<const LeftShift&>(object.value));
    }
    if (object.value.type() == typeid(RightShift)) {
        return eval(std::any_cast<const RightShift&>(object.value));
    }
    if (object.value.type() == typeid(Symbol)) {
        return eval(std::any_cast<const Symbol&>(object.value));
    }
    if (object.value.type() == typeid(Getter)) {
        return eval(std::any_cast<const Getter&>(object.value));
    }
    if (object.value.type() == typeid(Setter)) {
        return eval(std::any_cast<const Setter&>(object.value));
    }
    if (object.value.type() == typeid(Assignment)) {
        return eval(std::any_cast<const Assignment&>(object.value));
    }
    if (object.value.type() == typeid(List)) {
        return eval(std::any_cast<const List&>(object.value));
    }
    if (object.value.type() == typeid(Quote)) {
        return eval(std::any_cast<const Quote&>(object.value));
    }
    return object;
}

}  // namespace fundot
