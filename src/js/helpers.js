module.exports = {
    applyEnums: function(context, enums, label, idsLabel) {
        if (!idsLabel) idsLabel = `${label.slice(0, -1)}_IDS`;
        context[idsLabel] = enums;
        context[label] = Object.keys(enums).reduce((obj, key) => {
            obj[enums[key]] = key;
            return obj;
        }, {});
    }
};
