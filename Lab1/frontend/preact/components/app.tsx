import { h, Component } from 'preact';
import { Grid, Cell, Card } from 'preact-fluid';

import Header from './header/header';

interface MyProps {}
interface MyState {}

export default class App extends Component<MyProps, MyState> {
    /** Gets fired when the route changes.
     *	@param {Object} event		"change" event from [preact-router](http://git.io/preact-router)
     *	@param {string} event.url	The newly routed URL
     */
    currentUrl;

    handleRoute = e => {
        this.currentUrl = e.url;
    };
    // <GridRow extraSmall="10" small="10" medium="10" large="10">
    // </GridRow>

    render() {
        return (
            <div id="app">
                <Header />
                <div style={{ width: '100%', height: '96px' }} />
                <Grid columns={1} minRowHeight="80px" gap="50px">
                    <Cell center height={2}>
                        <Card style={{ height: '100%', width: '90%', marginLeft: '5%', marginRight: '5%', borderRadius: '10px' }} />
                    </Cell>
                    <Cell center middle height={1}>
                        <Card style={{ height: '100%', width: '90%', marginLeft: '5%', marginRight: '5%', borderRadius: '10px' }} />
                    </Cell>
                    <Cell center middle height={1}>
                        <Card style={{ height: '100%', width: '90%', marginLeft: '5%', marginRight: '5%', borderRadius: '10px' }} />
                    </Cell>
                </Grid>
                <div style={{ width: '100%', height: '96px' }} />
            </div>
        );
    }
}
